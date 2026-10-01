#!/usr/bin/env python3
# Copyright (c) 2026 Dm1stry
# SPDX-License-Identifier: MIT
"""Runs the benchmarks and writes their results into README.md.

Builds the `release` preset, runs every benchmark executable with its threads
pinned to two different physical cores (Linux only) and replaces the text
between the benchmark markers in README.md with one table per benchmark.

The tables rely on the naming used in tests/benchmarks:
  BM_<Name><queue type>[/<argument name>:<value>]...
A benchmark that reports items_per_second is shown as throughput, any other as
real time per iteration. Each cell is the median of the repetitions.
"""

import argparse
import datetime
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
README = ROOT / "README.md"
DEFAULT_BUILD_DIR = ROOT / "build" / "release"
BEGIN_MARKER = "<!-- benchmarks:begin -->"
END_MARKER = "<!-- benchmarks:end -->"

# How the baseline queues of tests/benchmarks are named in the tables; every
# other type is shown as ccc::<type>.
BASELINE_NAMES = {
    "baseline::boost_spsc_queue": "boost::lockfree::spsc_queue",
    "baseline::locked_queue": "std::deque + std::mutex",
}

# Name parts that describe how a benchmark was run rather than its input.
RUN_OPTION = re.compile(r"(real_time|process_time|manual_time|min_time:.*|repeats:.*|iterations:.*)")

NANOSECONDS_PER_UNIT = {"ns": 1, "us": 1e3, "ms": 1e6, "s": 1e9}


def run_command(command, **kwargs):
    print("+", " ".join(str(part) for part in command), flush=True)
    subprocess.run(command, check=True, **kwargs)


def build(build_dir):
    if build_dir == DEFAULT_BUILD_DIR:
        run_command(["cmake", "--preset", "release"], cwd=ROOT)
        run_command(["cmake", "--build", "--preset", "release"], cwd=ROOT)
    else:
        run_command(["cmake", "--build", str(build_dir), "--config", "Release"])


def find_benchmarks(build_dir):
    root = build_dir / "tests" / "benchmarks"
    binaries = sorted(
        path
        for path in root.rglob("*")
        if path.is_file()
        and path.stem.endswith("_benchmark")
        and path.suffix in ("", ".exe")
        and os.access(path, os.X_OK)
    )
    if not binaries:
        sys.exit(f"No benchmark executables under {root}; were the benchmarks built?")
    return binaries


def pick_cpus():
    """Returns two logical CPUs on different physical cores, skipping the core of CPU 0."""
    if platform.system() != "Linux" or not shutil.which("taskset"):
        return None
    cores = {}
    for topology in Path("/sys/devices/system/cpu").glob("cpu[0-9]*/topology"):
        core = tuple((topology / name).read_text().strip() for name in ("physical_package_id", "core_id"))
        cores.setdefault(core, []).append(int(topology.parent.name[3:]))
    candidates = [min(cpus) for cpus in sorted(cores.values(), key=min) if 0 not in cpus]
    return candidates[:2] if len(candidates) >= 2 else None


def run_benchmarks(binaries, cpus, repetitions):
    results = {"context": None, "benchmarks": []}
    with tempfile.TemporaryDirectory() as directory:
        for binary in binaries:
            output = Path(directory) / f"{binary.stem}.json"
            command = [
                str(binary),
                f"--benchmark_out={output}",
                "--benchmark_out_format=json",
                f"--benchmark_repetitions={repetitions}",
                "--benchmark_report_aggregates_only=true",
            ]
            if cpus:
                command = ["taskset", "-c", ",".join(map(str, cpus)), *command]
            run_command(command)
            data = json.loads(output.read_text())
            results["context"] = results["context"] or data["context"]
            results["benchmarks"] += data["benchmarks"]
    return results


def describe_cpu():
    try:
        if platform.system() == "Linux":
            for line in Path("/proc/cpuinfo").read_text().splitlines():
                if line.startswith("model name"):
                    return line.split(":", 1)[1].strip()
        elif platform.system() == "Darwin":
            command = ["sysctl", "-n", "machdep.cpu.brand_string"]
            return subprocess.run(command, capture_output=True, text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        pass
    return platform.processor() or "unknown CPU"


def describe_compiler(build_dir):
    names = {"GNU": "GCC"}
    for path in build_dir.glob("CMakeFiles/*/CMakeCXXCompiler.cmake"):
        text = path.read_text()
        compiler = re.search(r'set\(CMAKE_CXX_COMPILER_ID "([^"]+)"\)', text)
        version = re.search(r'set\(CMAKE_CXX_COMPILER_VERSION "([^"]+)"\)', text)
        if compiler and version:
            return f"{names.get(compiler[1], compiler[1])} {version[1]}"
    return None


def parse_name(run_name):
    """Splits 'BM_Throughput<Q<int>>/capacity:64/real_time' into ('Throughput', 'Q<int>', [('capacity', '64')])."""
    match = re.match(r"BM_(\w+)<", run_name)
    if not match:
        return None
    depth = 0
    for end in range(match.end() - 1, len(run_name)):
        depth += {"<": 1, ">": -1}.get(run_name[end], 0)
        if depth == 0:
            break
    arguments = []
    for part in run_name[end + 1 :].split("/"):
        if part and not RUN_OPTION.fullmatch(part):
            name, _, value = part.rpartition(":")
            arguments.append((name or "argument", value))
    return match[1], run_name[match.end() : end], arguments


def display_name(type_name):
    base = re.sub(r"<.*>", "", type_name)
    return BASELINE_NAMES.get(base, f"ccc::{base}")


def format_rate(per_second):
    for scale, suffix in ((1e9, " G"), (1e6, " M"), (1e3, " k")):
        if per_second >= scale:
            return f"{per_second / scale:.3g}{suffix}"
    return f"{per_second:.3g}"


def format_time(nanoseconds):
    for scale, suffix in ((1e9, " s"), (1e6, " ms"), (1e3, " µs")):
        if nanoseconds >= scale:
            return f"{nanoseconds / scale:.3g}{suffix}"
    return f"{nanoseconds:.3g} ns"


def render_table(family, table):
    rows = table["rows"]
    is_rate = all("items_per_second" in entry for row in rows.values() for entry in row.values())
    title = re.sub(r"(?<!^)(?=[A-Z])", " ", family).capitalize()
    unit = "items per second, higher is better" if is_rate else "time per iteration, lower is better"
    argument_names = [name for name, _ in next(iter(rows))]
    lines = [
        f"**{title}** ({unit})",
        "",
        "| " + " | ".join(argument_names + [f"`{column}`" for column in table["columns"]]) + " |",
        "|" + "---:|" * (len(argument_names) + len(table["columns"])),
    ]
    def value(entry):
        if is_rate:
            return entry["items_per_second"]
        return entry["real_time"] * NANOSECONDS_PER_UNIT[entry["time_unit"]]

    for arguments, row in rows.items():
        best =(max if is_rate else min)(value(entry) for entry in row.values())
        cells = [argument for _, argument in arguments]
        for column in table["columns"]:
            if column not in row:
                cells.append("")
                continue
            text = format_rate(value(row[column])) if is_rate else format_time(value(row[column]))
            cells.append(f"**{text}**" if value(row[column]) == best else text)
        lines.append("| " + " | ".join(cells) + " |")
    return "\n".join(lines)


def render(results):
    entries = [entry for entry in results["benchmarks"] if entry.get("aggregate_name") == "median"]
    entries = entries or [entry for entry in results["benchmarks"] if entry.get("run_type") == "iteration"]
    tables = {}
    for entry in entries:
        parsed = parse_name(entry["run_name"])
        if parsed is None:
            continue
        family, type_name, arguments = parsed
        table = tables.setdefault(family, {"columns": [], "rows": {}})
        column = display_name(type_name)
        if column not in table["columns"]:
            table["columns"].append(column)
        table["rows"].setdefault(tuple(arguments), {})[column] = entry

    run = results.get("ccc", {})
    setup = [run.get("cpu"), run.get("os"), run.get("compiler")]
    if run.get("cpus"):
        setup.append("threads pinned to CPUs " + " and ".join(map(str, run["cpus"])))
    if run.get("repetitions", 1) > 1:
        setup.append(f"median of {run['repetitions']} runs")
    header = (
        f"<!-- Generated by tools/update_benchmarks.py; do not edit by hand. -->\n"
        f"_Measured on {run.get('date', 'an unknown date')}: "
        + ", ".join(part for part in setup if part)
        + ". Absolute numbers depend on the machine; compare the columns._"
    )
    return "\n\n".join([header] + [render_table(family, table) for family, table in tables.items()])


def update_readme(markdown):
    text = README.read_text(encoding="utf-8")
    begin, end = text.find(BEGIN_MARKER), text.find(END_MARKER)
    if begin < 0 or end < begin:
        sys.exit(f"{README} has no {BEGIN_MARKER} ... {END_MARKER} block")
    begin += len(BEGIN_MARKER)
    README.write_text(text[:begin] + "\n" + markdown + "\n" + text[end:], encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD_DIR,
                        help="configured build directory with benchmarks (default: the release preset's)")
    parser.add_argument("--skip-build", action="store_true", help="use the benchmarks as they are")
    parser.add_argument("--cpus", help="CPUs to pin to, e.g. 2,4, or 'none' (default: two physical cores)")
    parser.add_argument("--repetitions", type=int, default=5, help="runs of each benchmark (default: 5)")
    parser.add_argument("--json", type=Path, help="render results saved by an earlier run instead of running")
    parser.add_argument("--print", action="store_true", help="print the tables instead of updating README.md")
    args = parser.parse_args()

    if args.json:
        results = json.loads(args.json.read_text())
    else:
        if not args.skip_build:
            build(args.build_dir)
        if args.cpus == "none":
            cpus = None
        elif args.cpus:
            cpus = [int(cpu) for cpu in args.cpus.split(",")]
        else:
            cpus = pick_cpus()
        if cpus and not shutil.which("taskset"):
            sys.exit("Pinning needs taskset, which is Linux only; pass --cpus none")
        results = run_benchmarks(find_benchmarks(args.build_dir), cpus, args.repetitions)
        results["ccc"] = {
            "date": datetime.date.today().isoformat(),
            "cpu": describe_cpu(),
            "os": platform.system(),
            "compiler": describe_compiler(args.build_dir),
            "cpus": cpus,
            "repetitions": args.repetitions,
        }
        saved = args.build_dir / "benchmark_results.json"
        saved.write_text(json.dumps(results, indent=2))
        print(f"Results saved to {saved}; re-render them with --json {saved}")

    markdown = render(results)
    if args.print:
        print(markdown)
    else:
        update_readme(markdown)
        print(f"Updated {README}")


if __name__ == "__main__":
    main()

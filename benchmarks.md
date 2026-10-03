+ taskset -c 2,4 /Data/File_Storage/Code/Crossplatform/concurrent-cpp-containers/build/release/tests/benchmarks/spsc_queue_benchmark --benchmark_out=/tmp/tmppoxal281/spsc_queue_benchmark.json --benchmark_out_format=json --benchmark_repetitions=10 --benchmark_report_aggregates_only=true
---------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                               Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------------------------------------------------------------------------
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:64/real_time_mean                               3.42 ns         3.41 ns           10 items_per_second=292.284M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:64/real_time_median                             3.40 ns         3.40 ns           10 items_per_second=293.765M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:64/real_time_stddev                            0.039 ns        0.037 ns           10 items_per_second=3.34516M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:64/real_time_cv                                 1.15 %          1.08 %            10 items_per_second=1.14%
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:256/real_time_mean                              3.22 ns         3.22 ns           10 items_per_second=319.075M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:256/real_time_median                            3.01 ns         3.00 ns           10 items_per_second=332.576M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:256/real_time_stddev                           0.708 ns        0.704 ns           10 items_per_second=45.0885M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:256/real_time_cv                               21.96 %         21.88 %            10 items_per_second=14.13%
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:4096/real_time_mean                             5.03 ns         5.02 ns           10 items_per_second=198.767M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:4096/real_time_median                           5.00 ns         4.99 ns           10 items_per_second=200.016M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:4096/real_time_stddev                          0.056 ns        0.055 ns           10 items_per_second=2.21065M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:4096/real_time_cv                               1.12 %          1.10 %            10 items_per_second=1.11%
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:65536/real_time_mean                            5.25 ns         5.24 ns           10 items_per_second=190.482M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:65536/real_time_median                          5.24 ns         5.23 ns           10 items_per_second=190.872M/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:65536/real_time_stddev                         0.026 ns        0.026 ns           10 items_per_second=952.753k/s
BM_Throughput<wait_free::spsc_queue<Value>>/capacity:65536/real_time_cv                              0.50 %          0.49 %            10 items_per_second=0.50%
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:64/real_time_mean                        3.73 ns         3.73 ns           10 items_per_second=267.972M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:64/real_time_median                      3.72 ns         3.71 ns           10 items_per_second=269.166M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:64/real_time_stddev                     0.040 ns        0.040 ns           10 items_per_second=2.85374M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:64/real_time_cv                          1.08 %          1.07 %            10 items_per_second=1.06% rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:256/real_time_mean                       3.59 ns         3.58 ns           10 items_per_second=278.652M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:256/real_time_median                     3.58 ns         3.57 ns           10 items_per_second=279.364M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:256/real_time_stddev                    0.029 ns        0.029 ns           10 items_per_second=2.26547M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:256/real_time_cv                         0.82 %          0.81 %            10 items_per_second=0.81% rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:4096/real_time_mean                      5.70 ns         5.69 ns           10 items_per_second=175.486M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:4096/real_time_median                    5.69 ns         5.68 ns           10 items_per_second=175.687M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:4096/real_time_stddev                   0.034 ns        0.034 ns           10 items_per_second=1.05602M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:4096/real_time_cv                        0.60 %          0.59 %            10 items_per_second=0.60% rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:65536/real_time_mean                     5.90 ns         5.89 ns           10 items_per_second=169.561M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:65536/real_time_median                   5.88 ns         5.87 ns           10 items_per_second=169.972M/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:65536/real_time_stddev                  0.028 ns        0.027 ns           10 items_per_second=788.608k/s rigtorp::SPSCQueue
BM_Throughput<baseline::rigtorp_spsc_queue<Value>>/capacity:65536/real_time_cv                       0.47 %          0.46 %            10 items_per_second=0.47% rigtorp::SPSCQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:64/real_time_mean            3.92 ns         3.91 ns           10 items_per_second=256.257M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:64/real_time_median          3.91 ns         3.90 ns           10 items_per_second=255.775M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:64/real_time_stddev         0.276 ns        0.276 ns           10 items_per_second=16.49M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:64/real_time_cv              7.05 %          7.04 %            10 items_per_second=6.43% moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:256/real_time_mean           6.46 ns         6.45 ns           10 items_per_second=160.07M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:256/real_time_median         6.79 ns         6.78 ns           10 items_per_second=147.238M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:256/real_time_stddev         1.09 ns         1.08 ns           10 items_per_second=34.8599M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:256/real_time_cv            16.80 %         16.81 %            10 items_per_second=21.78% moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:4096/real_time_mean          6.66 ns         6.65 ns           10 items_per_second=150.654M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:4096/real_time_median        6.80 ns         6.79 ns           10 items_per_second=147.139M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:4096/real_time_stddev       0.346 ns        0.345 ns           10 items_per_second=8.84844M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:4096/real_time_cv            5.20 %          5.20 %            10 items_per_second=5.87% moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:65536/real_time_mean         6.31 ns         6.30 ns           10 items_per_second=159.884M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:65536/real_time_median       6.46 ns         6.45 ns           10 items_per_second=154.68M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:65536/real_time_stddev      0.545 ns        0.544 ns           10 items_per_second=17.5817M/s moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_reader_writer_queue<Value>>/capacity:65536/real_time_cv           8.64 %          8.64 %            10 items_per_second=11.00% moodycamel::ReaderWriterQueue
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:64/real_time_mean                48.6 ns         48.5 ns           10 items_per_second=20.5741M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:64/real_time_median              48.8 ns         48.7 ns           10 items_per_second=20.5044M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:64/real_time_stddev             0.587 ns        0.588 ns           10 items_per_second=251.606k/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:64/real_time_cv                  1.21 %          1.21 %            10 items_per_second=1.22% moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:256/real_time_mean               43.6 ns         43.5 ns           10 items_per_second=22.9863M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:256/real_time_median             43.9 ns         43.8 ns           10 items_per_second=22.7702M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:256/real_time_stddev             1.63 ns         1.63 ns           10 items_per_second=921.662k/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:256/real_time_cv                 3.74 %          3.74 %            10 items_per_second=4.01% moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:4096/real_time_mean              53.7 ns         53.6 ns           10 items_per_second=18.6136M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:4096/real_time_median            53.7 ns         53.6 ns           10 items_per_second=18.612M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:4096/real_time_stddev           0.157 ns        0.157 ns           10 items_per_second=54.3429k/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:4096/real_time_cv                0.29 %          0.29 %            10 items_per_second=0.29% moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:65536/real_time_mean             54.0 ns         53.9 ns           10 items_per_second=18.5187M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:65536/real_time_median           54.1 ns         54.0 ns           10 items_per_second=18.498M/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:65536/real_time_stddev          0.197 ns        0.194 ns           10 items_per_second=67.4737k/s moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::moodycamel_circular_buffer<Value>>/capacity:65536/real_time_cv               0.36 %          0.36 %            10 items_per_second=0.36% moodycamel::BlockingReaderWriterCircularBuffer
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:64/real_time_mean                         15.8 ns         15.8 ns           10 items_per_second=63.2501M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:64/real_time_median                       15.8 ns         15.8 ns           10 items_per_second=63.2277M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:64/real_time_stddev                      0.177 ns        0.177 ns           10 items_per_second=712.57k/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:64/real_time_cv                           1.12 %          1.12 %            10 items_per_second=1.13% atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:256/real_time_mean                        15.9 ns         15.9 ns           10 items_per_second=62.9836M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:256/real_time_median                      15.8 ns         15.8 ns           10 items_per_second=63.1476M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:256/real_time_stddev                     0.120 ns        0.118 ns           10 items_per_second=473.703k/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:256/real_time_cv                          0.75 %          0.75 %            10 items_per_second=0.75% atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:4096/real_time_mean                       15.8 ns         15.8 ns           10 items_per_second=63.0957M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:4096/real_time_median                     15.8 ns         15.8 ns           10 items_per_second=63.0925M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:4096/real_time_stddev                    0.106 ns        0.107 ns           10 items_per_second=419.612k/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:4096/real_time_cv                         0.67 %          0.68 %            10 items_per_second=0.67% atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:65536/real_time_mean                      14.8 ns         14.8 ns           10 items_per_second=68.3832M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:65536/real_time_median                    15.3 ns         15.3 ns           10 items_per_second=65.2605M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:65536/real_time_stddev                    1.63 ns         1.62 ns           10 items_per_second=10.3154M/s atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::atomic_queue_spsc<Value>>/capacity:65536/real_time_cv                       10.96 %         10.96 %            10 items_per_second=15.08% atomic_queue::AtomicQueueB2 (SPSC)
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:64/real_time_mean                          5.71 ns         5.70 ns           10 items_per_second=175.207M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:64/real_time_median                        5.68 ns         5.67 ns           10 items_per_second=175.951M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:64/real_time_stddev                       0.070 ns        0.069 ns           10 items_per_second=2.11155M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:64/real_time_cv                            1.23 %          1.22 %            10 items_per_second=1.21% boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:256/real_time_mean                         5.73 ns         5.72 ns           10 items_per_second=174.603M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:256/real_time_median                       5.70 ns         5.69 ns           10 items_per_second=175.36M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:256/real_time_stddev                      0.059 ns        0.058 ns           10 items_per_second=1.77472M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:256/real_time_cv                           1.03 %          1.02 %            10 items_per_second=1.02% boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:4096/real_time_mean                        5.90 ns         5.89 ns           10 items_per_second=169.412M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:4096/real_time_median                      5.89 ns         5.88 ns           10 items_per_second=169.722M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:4096/real_time_stddev                     0.026 ns        0.025 ns           10 items_per_second=742.288k/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:4096/real_time_cv                          0.44 %          0.43 %            10 items_per_second=0.44% boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:65536/real_time_mean                       6.12 ns         6.11 ns           10 items_per_second=163.447M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:65536/real_time_median                     6.12 ns         6.10 ns           10 items_per_second=163.531M/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:65536/real_time_stddev                    0.025 ns        0.025 ns           10 items_per_second=667.4k/s boost::lockfree::spsc_queue
BM_Throughput<baseline::boost_spsc_queue<Value>>/capacity:65536/real_time_cv                         0.41 %          0.40 %            10 items_per_second=0.41% boost::lockfree::spsc_queue
BM_Throughput<DnedicQueue<64>>/capacity:64/real_time_mean                                            6.30 ns         6.29 ns           10 items_per_second=158.64M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<64>>/capacity:64/real_time_median                                          6.29 ns         6.28 ns           10 items_per_second=158.878M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<64>>/capacity:64/real_time_stddev                                         0.063 ns        0.063 ns           10 items_per_second=1.57566M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<64>>/capacity:64/real_time_cv                                              1.00 %          1.00 %            10 items_per_second=0.99% lockfree::spsc::Queue
BM_Throughput<DnedicQueue<256>>/capacity:256/real_time_mean                                          6.54 ns         6.52 ns           10 items_per_second=153.007M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<256>>/capacity:256/real_time_median                                        6.52 ns         6.51 ns           10 items_per_second=153.429M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<256>>/capacity:256/real_time_stddev                                       0.053 ns        0.053 ns           10 items_per_second=1.22476M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<256>>/capacity:256/real_time_cv                                            0.81 %          0.81 %            10 items_per_second=0.80% lockfree::spsc::Queue
BM_Throughput<DnedicQueue<4096>>/capacity:4096/real_time_mean                                        6.15 ns         6.13 ns           10 items_per_second=162.726M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<4096>>/capacity:4096/real_time_median                                      6.15 ns         6.14 ns           10 items_per_second=162.602M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<4096>>/capacity:4096/real_time_stddev                                     0.031 ns        0.032 ns           10 items_per_second=826.15k/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<4096>>/capacity:4096/real_time_cv                                          0.51 %          0.52 %            10 items_per_second=0.51% lockfree::spsc::Queue
BM_Throughput<DnedicQueue<65536>>/capacity:65536/real_time_mean                                      6.34 ns         6.32 ns           10 items_per_second=157.872M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<65536>>/capacity:65536/real_time_median                                    6.34 ns         6.33 ns           10 items_per_second=157.714M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<65536>>/capacity:65536/real_time_stddev                                   0.076 ns        0.075 ns           10 items_per_second=1.90207M/s lockfree::spsc::Queue
BM_Throughput<DnedicQueue<65536>>/capacity:65536/real_time_cv                                        1.20 %          1.19 %            10 items_per_second=1.20% lockfree::spsc::Queue
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:64/real_time_mean                   3.18 ns         3.17 ns           10 items_per_second=314.473M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:64/real_time_median                 3.17 ns         3.17 ns           10 items_per_second=315.236M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:64/real_time_stddev                0.026 ns        0.026 ns           10 items_per_second=2.56869M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:64/real_time_cv                     0.83 %          0.81 %            10 items_per_second=0.82% cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:256/real_time_mean                  4.41 ns         4.40 ns           10 items_per_second=226.842M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:256/real_time_median                4.41 ns         4.40 ns           10 items_per_second=226.557M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:256/real_time_stddev               0.091 ns        0.090 ns           10 items_per_second=4.75371M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:256/real_time_cv                    2.06 %          2.05 %            10 items_per_second=2.10% cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:4096/real_time_mean                 5.36 ns         5.36 ns           10 items_per_second=186.475M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:4096/real_time_median               5.36 ns         5.35 ns           10 items_per_second=186.675M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:4096/real_time_stddev              0.021 ns        0.021 ns           10 items_per_second=742.79k/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:4096/real_time_cv                   0.40 %          0.40 %            10 items_per_second=0.40% cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:65536/real_time_mean                5.44 ns         5.43 ns           10 items_per_second=183.808M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:65536/real_time_median              5.42 ns         5.42 ns           10 items_per_second=184.398M/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:65536/real_time_stddev             0.029 ns        0.029 ns           10 items_per_second=971.691k/s cds::container::WeakRingBuffer
BM_Throughput<baseline::libcds_weak_ring_buffer<Value>>/capacity:65536/real_time_cv                  0.53 %          0.53 %            10 items_per_second=0.53% cds::container::WeakRingBuffer
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:64/real_time_mean                       49.5 ns         49.4 ns           10 items_per_second=20.2178M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:64/real_time_median                     49.5 ns         49.4 ns           10 items_per_second=20.214M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:64/real_time_stddev                    0.352 ns        0.348 ns           10 items_per_second=143.697k/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:64/real_time_cv                         0.71 %          0.71 %            10 items_per_second=0.71% xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:256/real_time_mean                      48.4 ns         48.3 ns           10 items_per_second=20.6745M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:256/real_time_median                    48.3 ns         48.3 ns           10 items_per_second=20.6919M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:256/real_time_stddev                   0.264 ns        0.262 ns           10 items_per_second=112.07k/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:256/real_time_cv                        0.55 %          0.54 %            10 items_per_second=0.54% xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:4096/real_time_mean                     55.6 ns         55.5 ns           10 items_per_second=17.9926M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:4096/real_time_median                   55.7 ns         55.7 ns           10 items_per_second=17.9428M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:4096/real_time_stddev                  0.571 ns        0.592 ns           10 items_per_second=185.932k/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:4096/real_time_cv                       1.03 %          1.07 %            10 items_per_second=1.03% xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:65536/real_time_mean                    54.2 ns         54.1 ns           10 items_per_second=18.4652M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:65536/real_time_median                  54.2 ns         54.1 ns           10 items_per_second=18.4599M/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:65536/real_time_stddev                 0.415 ns        0.412 ns           10 items_per_second=142.208k/s xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_vyukov_queue<Value>>/capacity:65536/real_time_cv                      0.77 %          0.76 %            10 items_per_second=0.77% xenium::vyukov_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:64/real_time_mean                     76.6 ns         76.4 ns           10 items_per_second=13.0567M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:64/real_time_median                   76.4 ns         76.2 ns           10 items_per_second=13.089M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:64/real_time_stddev                  0.504 ns        0.494 ns           10 items_per_second=85.3961k/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:64/real_time_cv                       0.66 %          0.65 %            10 items_per_second=0.65% xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:256/real_time_mean                    79.6 ns         79.4 ns           10 items_per_second=12.5696M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:256/real_time_median                  79.6 ns         79.4 ns           10 items_per_second=12.5661M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:256/real_time_stddev                 0.876 ns        0.876 ns           10 items_per_second=138.216k/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:256/real_time_cv                      1.10 %          1.10 %            10 items_per_second=1.10% xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:4096/real_time_mean                    161 ns          161 ns           10 items_per_second=6.21792M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:4096/real_time_median                  159 ns          159 ns           10 items_per_second=6.26979M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:4096/real_time_stddev                 2.55 ns         2.53 ns           10 items_per_second=96.5876k/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:4096/real_time_cv                     1.58 %          1.58 %            10 items_per_second=1.55% xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:65536/real_time_mean                   160 ns          159 ns           10 items_per_second=6.26212M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:65536/real_time_median                 159 ns          159 ns           10 items_per_second=6.27204M/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:65536/real_time_stddev                4.11 ns         4.10 ns           10 items_per_second=157.063k/s xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::xenium_nikolaev_queue<Value>>/capacity:65536/real_time_cv                    2.57 %          2.57 %            10 items_per_second=2.51% xenium::nikolaev_bounded_queue (MPMC)
BM_Throughput<baseline::locked_queue<Value>>/capacity:64/real_time_mean                               159 ns          158 ns           10 items_per_second=6.30063M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:64/real_time_median                             158 ns          158 ns           10 items_per_second=6.31817M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:64/real_time_stddev                            6.94 ns         6.92 ns           10 items_per_second=268.899k/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:64/real_time_cv                                4.36 %          4.37 %            10 items_per_second=4.27% std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:256/real_time_mean                              102 ns          101 ns           10 items_per_second=9.91169M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:256/real_time_median                            102 ns          102 ns           10 items_per_second=9.7628M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:256/real_time_stddev                           9.09 ns         9.06 ns           10 items_per_second=930.305k/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:256/real_time_cv                               8.94 %          8.94 %            10 items_per_second=9.39% std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:4096/real_time_mean                            93.0 ns         92.7 ns           10 items_per_second=11.0642M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:4096/real_time_median                          89.3 ns         88.9 ns           10 items_per_second=11.2037M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:4096/real_time_stddev                          19.2 ns         19.1 ns           10 items_per_second=1.74107M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:4096/real_time_cv                             20.61 %         20.60 %            10 items_per_second=15.74% std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:65536/real_time_mean                           83.3 ns         83.0 ns           10 items_per_second=12.2894M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:65536/real_time_median                         81.8 ns         81.5 ns           10 items_per_second=12.2344M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:65536/real_time_stddev                         13.1 ns         13.1 ns           10 items_per_second=2.00773M/s std::deque + std::mutex
BM_Throughput<baseline::locked_queue<Value>>/capacity:65536/real_time_cv                            15.79 %         15.79 %            10 items_per_second=16.34% std::deque + std::mutex
BM_RoundTrip<wait_free::spsc_queue<Value>>/real_time_mean                                             131 ns          131 ns           10
BM_RoundTrip<wait_free::spsc_queue<Value>>/real_time_median                                           131 ns          130 ns           10
BM_RoundTrip<wait_free::spsc_queue<Value>>/real_time_stddev                                          2.13 ns         2.11 ns           10
BM_RoundTrip<wait_free::spsc_queue<Value>>/real_time_cv                                              1.62 %          1.61 %            10
BM_RoundTrip<baseline::rigtorp_spsc_queue<Value>>/real_time_mean                                      134 ns          134 ns           10 rigtorp::SPSCQueue
BM_RoundTrip<baseline::rigtorp_spsc_queue<Value>>/real_time_median                                    134 ns          133 ns           10 rigtorp::SPSCQueue
BM_RoundTrip<baseline::rigtorp_spsc_queue<Value>>/real_time_stddev                                  0.677 ns        0.668 ns           10 rigtorp::SPSCQueue
BM_RoundTrip<baseline::rigtorp_spsc_queue<Value>>/real_time_cv                                       0.51 %          0.50 %            10 rigtorp::SPSCQueue
BM_RoundTrip<baseline::moodycamel_reader_writer_queue<Value>>/real_time_mean                          133 ns          133 ns           10 moodycamel::ReaderWriterQueue
BM_RoundTrip<baseline::moodycamel_reader_writer_queue<Value>>/real_time_median                        132 ns          132 ns           10 moodycamel::ReaderWriterQueue
BM_RoundTrip<baseline::moodycamel_reader_writer_queue<Value>>/real_time_stddev                       1.35 ns         1.34 ns           10 moodycamel::ReaderWriterQueue
BM_RoundTrip<baseline::moodycamel_reader_writer_queue<Value>>/real_time_cv                           1.01 %          1.01 %            10 moodycamel::ReaderWriterQueue
BM_RoundTrip<baseline::moodycamel_circular_buffer<Value>>/real_time_mean                              204 ns          204 ns           10 moodycamel::BlockingReaderWriterCircularBuffer
BM_RoundTrip<baseline::moodycamel_circular_buffer<Value>>/real_time_median                            203 ns          203 ns           10 moodycamel::BlockingReaderWriterCircularBuffer
BM_RoundTrip<baseline::moodycamel_circular_buffer<Value>>/real_time_stddev                          0.905 ns        0.891 ns           10 moodycamel::BlockingReaderWriterCircularBuffer
BM_RoundTrip<baseline::moodycamel_circular_buffer<Value>>/real_time_cv                               0.44 %          0.44 %            10 moodycamel::BlockingReaderWriterCircularBuffer
BM_RoundTrip<baseline::atomic_queue_spsc<Value>>/real_time_mean                                       147 ns          147 ns           10 atomic_queue::AtomicQueueB2 (SPSC)
BM_RoundTrip<baseline::atomic_queue_spsc<Value>>/real_time_median                                     146 ns          146 ns           10 atomic_queue::AtomicQueueB2 (SPSC)
BM_RoundTrip<baseline::atomic_queue_spsc<Value>>/real_time_stddev                                    1.27 ns         1.25 ns           10 atomic_queue::AtomicQueueB2 (SPSC)
BM_RoundTrip<baseline::atomic_queue_spsc<Value>>/real_time_cv                                        0.87 %          0.85 %            10 atomic_queue::AtomicQueueB2 (SPSC)
BM_RoundTrip<baseline::boost_spsc_queue<Value>>/real_time_mean                                        141 ns          141 ns           10 boost::lockfree::spsc_queue
BM_RoundTrip<baseline::boost_spsc_queue<Value>>/real_time_median                                      140 ns          140 ns           10 boost::lockfree::spsc_queue
BM_RoundTrip<baseline::boost_spsc_queue<Value>>/real_time_stddev                                    0.974 ns        0.956 ns           10 boost::lockfree::spsc_queue
BM_RoundTrip<baseline::boost_spsc_queue<Value>>/real_time_cv                                         0.69 %          0.68 %            10 boost::lockfree::spsc_queue
BM_RoundTrip<DnedicQueue<64>>/real_time_mean                                                          140 ns          140 ns           10 lockfree::spsc::Queue
BM_RoundTrip<DnedicQueue<64>>/real_time_median                                                        139 ns          139 ns           10 lockfree::spsc::Queue
BM_RoundTrip<DnedicQueue<64>>/real_time_stddev                                                      0.724 ns        0.702 ns           10 lockfree::spsc::Queue
BM_RoundTrip<DnedicQueue<64>>/real_time_cv                                                           0.52 %          0.50 %            10 lockfree::spsc::Queue
BM_RoundTrip<baseline::libcds_weak_ring_buffer<Value>>/real_time_mean                                 146 ns          145 ns           10 cds::container::WeakRingBuffer
BM_RoundTrip<baseline::libcds_weak_ring_buffer<Value>>/real_time_median                               145 ns          145 ns           10 cds::container::WeakRingBuffer
BM_RoundTrip<baseline::libcds_weak_ring_buffer<Value>>/real_time_stddev                             0.803 ns        0.778 ns           10 cds::container::WeakRingBuffer
BM_RoundTrip<baseline::libcds_weak_ring_buffer<Value>>/real_time_cv                                  0.55 %          0.54 %            10 cds::container::WeakRingBuffer
BM_RoundTrip<baseline::xenium_vyukov_queue<Value>>/real_time_mean                                     187 ns          187 ns           10 xenium::vyukov_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_vyukov_queue<Value>>/real_time_median                                   186 ns          186 ns           10 xenium::vyukov_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_vyukov_queue<Value>>/real_time_stddev                                  1.23 ns         1.10 ns           10 xenium::vyukov_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_vyukov_queue<Value>>/real_time_cv                                      0.66 %          0.59 %            10 xenium::vyukov_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_nikolaev_queue<Value>>/real_time_mean                                   470 ns          469 ns           10 xenium::nikolaev_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_nikolaev_queue<Value>>/real_time_median                                 469 ns          468 ns           10 xenium::nikolaev_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_nikolaev_queue<Value>>/real_time_stddev                                2.75 ns         2.72 ns           10 xenium::nikolaev_bounded_queue (MPMC)
BM_RoundTrip<baseline::xenium_nikolaev_queue<Value>>/real_time_cv                                    0.59 %          0.58 %            10 xenium::nikolaev_bounded_queue (MPMC)
BM_RoundTrip<baseline::locked_queue<Value>>/real_time_mean                                           1053 ns         1049 ns           10 std::deque + std::mutex
BM_RoundTrip<baseline::locked_queue<Value>>/real_time_median                                         1054 ns         1050 ns           10 std::deque + std::mutex
BM_RoundTrip<baseline::locked_queue<Value>>/real_time_stddev                                         5.32 ns         5.30 ns           10 std::deque + std::mutex
BM_RoundTrip<baseline::locked_queue<Value>>/real_time_cv                                             0.50 %          0.51 %            10 std::deque + std::mutex
Results saved to /Data/File_Storage/Code/Crossplatform/concurrent-cpp-containers/build/release/benchmark_results.json; re-render them with --json /Data/File_Storage/Code/Crossplatform/concurrent-cpp-containers/build/release/benchmark_results.json
<!-- Generated by tools/update_benchmarks.py; do not edit by hand. -->
_Measured on 2026-10-03: AMD Ryzen 7 8845H w/ Radeon 780M Graphics, Linux, GCC 16.2.1, threads pinned to CPUs 2 and 4, median of 10 runs ± coefficient of variation (standard deviation / mean). Absolute numbers depend on the machine; compare the queues with each other._

**Throughput** (items per second by capacity, higher is better)

| Queue | 64 | 256 | 4096 | 65536 |
|---|---:|---:|---:|---:|
| `ccc::wait_free::spsc_queue` | 294 M ±1% | **333 M** ±14% | **200 M** ±1% | **191 M** ±0.5% |
| `rigtorp::SPSCQueue` | 269 M ±1% | 279 M ±0.8% | 176 M ±0.6% | 170 M ±0.5% |
| `moodycamel::ReaderWriterQueue` | 256 M ±6% | 147 M ±22% | 147 M ±6% | 155 M ±11% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 20.5 M ±1% | 22.8 M ±4% | 18.6 M ±0.3% | 18.5 M ±0.4% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 63.2 M ±1% | 63.1 M ±0.8% | 63.1 M ±0.7% | 65.3 M ±15% |
| `boost::lockfree::spsc_queue` | 176 M ±1% | 175 M ±1% | 170 M ±0.4% | 164 M ±0.4% |
| `lockfree::spsc::Queue` | 159 M ±1.0% | 153 M ±0.8% | 163 M ±0.5% | 158 M ±1% |
| `cds::container::WeakRingBuffer` | **315 M** ±0.8% | 227 M ±2% | 187 M ±0.4% | 184 M ±0.5% |
| `xenium::vyukov_bounded_queue (MPMC)` | 20.2 M ±0.7% | 20.7 M ±0.5% | 17.9 M ±1% | 18.5 M ±0.8% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 13.1 M ±0.7% | 12.6 M ±1% | 6.27 M ±2% | 6.27 M ±3% |
| `std::deque + std::mutex` | 6.32 M ±4% | 9.76 M ±9% | 11.2 M ±16% | 12.2 M ±16% |

**Round trip** (time per iteration, lower is better)

| Queue | Time per iteration |
|---|---:|
| `ccc::wait_free::spsc_queue` | **131 ns** ±2% |
| `rigtorp::SPSCQueue` | 134 ns ±0.5% |
| `moodycamel::ReaderWriterQueue` | 132 ns ±1% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 203 ns ±0.4% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 146 ns ±0.9% |
| `boost::lockfree::spsc_queue` | 140 ns ±0.7% |
| `lockfree::spsc::Queue` | 139 ns ±0.5% |
| `cds::container::WeakRingBuffer` | 145 ns ±0.6% |
| `xenium::vyukov_bounded_queue (MPMC)` | 186 ns ±0.7% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 469 ns ±0.6% |
| `std::deque + std::mutex` | 1.05 µs ±0.5% |

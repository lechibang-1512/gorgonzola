# Click Benchmarks

We use ClickBench datasets and the set of queries which are adapted to Gorgonzola's Cypher query language.

At the moment, the benchmarks are only working with the CSV dataset, however this must be loaded in single-threaded mode since there are quoted newlines, so it is very slow to load (using parquet would be faster, but the official parquet file stores timestamps in int64 columns which we don't convert implicitly.

The script benchmark.sh will run the entire benchmark pipeline, including downloading and importing the dataset.

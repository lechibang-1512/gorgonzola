# Gorgonzola

Gorgonzola is an embedded, serverless graph database engineered for analytical query speed and scalability. It is a clean fork of Kùzu — a project that has since been archived by its original authors — and carries the codebase forward with C/C++ API extensions and performance improvements in a unified, maintainable form.

## Features

- **Property Graph Model** — flexible node and relationship schema with Cypher query language
- **Embeddable & Serverless** — link directly into your application, no separate server process
- **Full-Text Search** — native FTS index for text-heavy workloads
- **Vector Index** — built-in HNSW index for approximate nearest-neighbor search
- **Columnar Storage** — disk-based columnar layout optimized for analytical scans
- **CSR Join Indices** — columnar sparse row-based adjacency list for fast graph traversal
- **Vectorized Query Engine** — factorized, vectorized execution with novel join algorithms
- **Multi-core Parallelism** — parallel query execution across all available cores
- **ACID Transactions** — full serializable transaction support
- **WebAssembly Bindings** — run Gorgonzola securely in the browser via Wasm

## What This Fork Adds

Gorgonzola is API-compatible with Kùzu and layers in the following improvements:

### C/C++ API Extensions

| Addition | Description |
|---|---|
| `uint128_t` support | Full 128-bit unsigned integer type alongside `int128_t` and `uuid`, with arithmetic, comparison, and cast operators |
| `gorgonzola_value_get_blob_with_size` | Null-byte-safe BLOB retrieval — the original `get_blob` truncates binary data at the first `\0`; this API returns the full buffer and its byte length |

### Cypher Language

- **Leading zeros in numeric literals** — `00123` is now a valid integer literal (previously rejected by the grammar)
- **Standardized vector function signatures** — consistent naming and argument ordering across all vector operations


## Getting Started

Clone the repository and refer to the developer guide for build instructions, then explore the `examples/` directory for sample queries and usage patterns.

## Extensions

Gorgonzola supports runtime-loadable extensions.

**v0.11.3+:** The `algo`, `fts`, `json`, and `vector` extensions are pre-installed. No `INSTALL` command needed.

**Older versions:** You must host a local extension server and install extensions manually.

### Hosting a local extension server

The extension server is an NGINX-based Docker container that serves extension binaries.

```bash
docker build -t gorgonzola-extension-server -f build-context/Dockerfile .
docker run -d -p 8080:80 gorgonzola-extension-server
```

Once running, install extensions by pointing `INSTALL` at your local server:

```cypher
INSTALL <EXTENSION_NAME> FROM 'http://localhost:8080/';
```

## Building from Source

See [CONTRIBUTING.md](CONTRIBUTING.md) for build prerequisites and instructions.

## License

[MIT](LICENSE)

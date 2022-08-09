# Attester for PoC

## Start

Using cargo to run test
```bash
RUSTFLAGS="-C link-args=-Wl,-rpath,/usr/local/lib/librats"  cargo run --bin attester -- --ac-addr 0.0.0.0:8001
```

Then the attester-server will be launched and listening to port 8001
for connections from Attestation-Client.

Also, you can build the binary

```bash
RUSTFLAGS="-C link-args=-Wl,-rpath,/usr/local/lib/librats" cargo build
```

Then execute with environment variable to enable log

```bash
<path-to-attester> --ac-addr 0.0.0.0:8001
```
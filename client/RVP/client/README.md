# Control Client for Reference Value Provider Service

launch a client
```bash
cargo run -- --rvps-addr http://127.0.0.1:8000 --path ../tests/message/intoto.json
```

Here, the `rvps_addr` parameter of client side must have a
`http://` prefix.
use std::fs;
use lib::{verify_evidence, AttestationEv};
use eventlog_rs::Eventlog;
use anyhow::*;

#[tokio::main]
async fn main() -> Result<()> {
    let report_data = "test".as_bytes().to_vec();
    let quote = fs::read("../../fixtures/quote")?;
    let eventlog_info = fs::read("../../fixtures/eventlog_info")?;
    let eventlog_data = fs::read("../../fixtures/eventlog_data")?;

    let evidence = AttestationEv {
        quote,
        eventlog_info,
        eventlog_data,
    };

    let eventlog = verify_evidence("tdx", evidence, &report_data).await?;

    println!("{:?}", eventlog);

    Ok(())
}

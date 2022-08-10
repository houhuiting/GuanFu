//! verifier

use anyhow::{anyhow, Result};
use eventlog_rs::Eventlog;
use lib::AttestationEv;
use reference_value_provider_service::ReferenceValue;

pub const REPORT_DATA: &[u8] = b"";
pub const TEE: &str = "tdx";

pub async fn verify_evidence(
    tee: &str,
    evidence: AttestationEv,
    report_data: &[u8],
) -> Result<Eventlog> {
    Err(anyhow!("No event log got"))
}

pub fn verify(event_log: Eventlog, rv: ReferenceValue) -> Result<()> {
    Ok(())
}

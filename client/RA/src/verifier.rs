//! verifier

use anyhow::Result;
use reference_value_provider_service::ReferenceValue;

pub const REPORT_DATA: &[u8] = b"";
pub const TEE: &str = "tdx";

pub async fn verify_evidence(
    tee: &str,
    evidence: AttestationEv,
    report_data: &[u8],
) -> Result<Eventlog> {
    anyhow!("No event log got")
}

pub fn verify(event_log: Eventlog, rv: ReferenceValue) -> Result<()> {
    Ok(())
}

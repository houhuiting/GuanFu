use anyhow::{anyhow, Result};
use eventlog_rs::Eventlog;
use librats_rs::{TeeType, get_quote, verify_quote};
use serde::{Serialize, Deserialize};

use std::fs;

const EVENTLOG_INFO_PATH: &str = "/sys/firmware/acpi/tables/TDEL";
const EVENTLOG_DATA_PATH: &str = "/sys/firmware/acpi/tables/data/TDEL";

#[derive(Serialize, Deserialize)]
pub struct AttestationEv {
    pub quote: Vec<u8>,
    pub eventlog_info: Vec<u8>,
    pub eventlog_data: Vec<u8>,
}

pub async fn get_evidence(report_data: Vec<u8>) -> Result<AttestationEv> {
    let quote = get_quote(&report_data).await?;
    let eventlog_info = fs::read(EVENTLOG_INFO_PATH).map_err(|e| {
        anyhow!(
            "Read eventlog info path {} failed: {:?}",
            EVENTLOG_INFO_PATH,
            e
        )
    })?;
    let eventlog_data = fs::read(EVENTLOG_DATA_PATH).map_err(|e| {
        anyhow!(
            "Read eventlog data path {} failed: {:?}",
            EVENTLOG_DATA_PATH,
            e
        )
    })?;
    Ok(AttestationEv {
        quote,
        eventlog_info,
        eventlog_data,
    })
}

pub async fn verify_evidence(
    tee: &str,
    evidence: AttestationEv,
    report_data: &[u8],
) -> Result<Eventlog> {
    if tee != "tdx" {
        return Err(anyhow!("Unsupported TEE type"));
    }

    let _parsed_claims = verify_quote(&evidence.quote, report_data, TeeType::TDX).await?;

    let event_log = Eventlog::try_from(evidence.eventlog_data.clone())?;
    // TODO: Verify the integrity of eventlog.

    Ok(event_log)
}
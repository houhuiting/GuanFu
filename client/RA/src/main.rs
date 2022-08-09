//! Attestation-Client for Guanfu

use anyhow::*;
use attestation::remote_attestation_client::RemoteAttestationClient;
use attestation::TcbStatusReq;
use clap::Parser;
use log::{error, info};
use query::query_reference_value_client::QueryReferenceValueClient;
use query::QueryReq;
use reference_value_provider_service::ReferenceValue;

pub mod query {
    tonic::include_proto!("query");
}

pub mod attestation {
    tonic::include_proto!("attestation");
}

mod verifier;

#[derive(Parser, Debug)]
#[clap(author, version, about, long_about = None)]
struct Args {
    #[clap(short, long, value_parser)]
    rvps_addr: String,

    #[clap(short, long, value_parser)]
    as_addr: String,
}

async fn real_main() -> Result<()> {
    let args = Args::parse();

    println!("rvps addr: {}", args.rvps_addr);
    println!("as addr: {}", args.as_addr);

    let as_addr = args.as_addr;
    let rvps_addr = args.rvps_addr;

    // get evidences
    let mut as_client = RemoteAttestationClient::connect(as_addr).await?;
    let query = TcbStatusReq {};
    let evi = as_client.get_tcb_status(query).await?.into_inner().status;

    let evi = serde_json::from_str(&evi)?;
    let event_log = verifier::verify_evidence(verifier::TEE, evi, verifier::REPORT_DATA).await?;

    // get reference values
    let mut rv_client = QueryReferenceValueClient::connect(rvps_addr).await?;
    let name = String::new();
    let query = QueryReq { name };
    let _rv = match rv_client.query(query).await?.into_inner().reference_value {
        None => return Err(anyhow!("No reference value find.")),
        Some(r) => serde_json::from_str::<ReferenceValue>(&r)?,
    };

    // compare

    Ok(())
}

#[tokio::main]
async fn main() {
    match real_main().await {
        std::result::Result::Ok(_) => info!("attestation succeed!"),
        Err(e) => error!("attestation failed: {}", e.to_string()),
    }
}

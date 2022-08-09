fn main() -> shadow_rs::SdResult<()> {
    println!("cargo:rerun-if-changed=../build.rs");
    tonic_build::compile_protos("../protos/attestation.proto")?;
    shadow_rs::new()
}

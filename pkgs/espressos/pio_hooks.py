import subprocess
print("Running protobuf code generation...")
subprocess.run(["make", "generate"], check=True)

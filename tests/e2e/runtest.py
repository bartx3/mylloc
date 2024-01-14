import subprocess

# Run the C program
process = subprocess.Popen(['./program'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Wait for the program to finish and get the stdout and stderr
stdout, stderr = process.communicate()

# Check for segmentation fault
if process.returncode == -11:
    print("Segmentation fault occurred")

# Print the stdout and stderr
print("STDOUT:")
print(stdout.decode())
print("STDERR:")
print(stderr.decode())

# Parse the stdout for allocator statistics
# This will depend on how your statistics are formatted
# Here's a simple example that assumes each statistic is printed on its own line in the format "Statistic: Value"
statistics = {}
for line in stdout.decode().split('\n'):
    if ': ' in line:
        stat, value = line.split(': ')
        statistics[stat] = value

# Print the statistics
for stat, value in statistics.items():
    print(f"{stat}: {value}")
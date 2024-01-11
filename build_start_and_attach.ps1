Write-Host "Building the container..." -ForegroundColor Blue
$buildOutput = docker-compose build --no-cache 2>&1
# Check if build was successful
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed with the following error:" -ForegroundColor Red
    # Extract error messages and format them
    $errors = ($buildOutput -split "`r`n" | Select-String -Pattern "error:" -Context 0,1)
    foreach ($error in $errors) {
        # Remove Docker build IDs and timestamps
        $cleanLine = ($error.Line -replace "#\d+\s+\d+\.\d+\s+", "")
        $cleanContext = $error.Context.PostContext -replace "#\d+\s+\d+\.\d+\s+", ""
        Write-Host "Error:" -ForegroundColor Red
        Write-Host $cleanLine -ForegroundColor Yellow
        $cleanContext | ForEach-Object { Write-Host $_ }
        Write-Host "" # Blank line for better readability
    }
    exit $LASTEXITCODE
}
# Start the containers in the background
docker-compose up -d
# Display a message
Write-Host "Waiting for the app container to start..." -ForegroundColor Blue
# Get the container ID of the app service
$appContainerId = docker ps -qf "name=something_new-app-1"
# Display a message
Write-Host "Attaching to the app container..." -ForegroundColor Blue
# Attach to the app container
Write-Host "------------------------------------------" -ForegroundColor Green
docker attach $appContainerId
<#
.SYNOPSIS
Generates a folder structure and captures file information.

.DESCRIPTION
This script generates a folder structure and captures file information within the specified depth of the directory tree. It lists the directory structure and files recursively, and saves the folder structure to a text file.

.PARAMETER depth
Specifies the depth of the directory tree to capture.

.EXAMPLE
.\generate_structure.ps1 -depth 3
Generates a folder structure and captures file information within a directory tree depth of 3.
#>

# Set the depth of the directory tree you want to capture
$depth = 3

# Get the current directory
$rootFolder = Get-Location

# Create an empty list to hold the folder structure
$folderStructure = @()

# Recursively list the directory structure and files
Get-ChildItem -Path $rootFolder -Recurse -Depth $depth | ForEach-Object {
    # Check if the item is a directory
    if ($_.PSIsContainer) {
        $folderStructure += $_.FullName.Replace($rootFolder, '') + "\"
    } else {
        # It's a file, add it with indentation
        $filePath = $_.FullName.Replace($rootFolder, '')
        $fileContent = Get-Content $_.FullName
        $lineCount = $fileContent.Count
        $tokenCount = ($fileContent | Measure-Object -Word).Words
        $avgTokensPerLine = if ($lineCount -gt 0) { $tokenCount / $lineCount } else { 0 }
        $roundedAvgTokensPerLine = [math]::Round($avgTokensPerLine, 2)
        $folderStructure += "    $filePath (Lines: $lineCount, Avg Tokens/Line: $roundedAvgTokensPerLine)"
    }
}

# Save the folder structure to a text file
$folderStructure | Out-File "folder_structure.txt"

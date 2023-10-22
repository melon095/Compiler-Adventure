dotnet clean
dotnet build -c Release

$testCases = Get-ChildItem -Path .\Examples -Filter *.lox -Recurse

foreach ($testCase in $testCases) {
    $out = .\bin\Release\net7.0\JLox.exe $testCase.FullName 2>&1
    $stderr = $out | ?{ $_ -is [System.Management.Automation.ErrorRecord] }
    
    if ($stderr) {
        Write-Host "Test failed: $($testCase.FullName)" -ForegroundColor Red
    }
    else
    {
        Write-Host "Test passed: $($testCase.FullName)" -ForegroundColor Green
    }
}
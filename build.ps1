$buildDir = "build"
$binDir = "bin"

New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

cmake -S . -B $buildDir -DCMAKE_BUILD_TYPE=Release --build $buildDir --config Release

if (!(Test-Path -Path $binDir -PathType Container)) {
    New-Item -ItemType Directory -Path $binDir -Force | Out-Null
}

New-Item -ItemType SymbolicLink -Path "$binDir/yurei" -Target "$buildDir/bin/yurei" -Force | Out-Null


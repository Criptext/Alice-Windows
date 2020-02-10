# =======================================================================
# CHECK   libssl1.0-dev
# =======================================================================
Try {
    mkdir deps
    Set-Location deps
}
Catch {
    Write-Host $_.Exception.Message`n
    exit 1
}
# =======================================================================
git config --global core.autocrlf false
# =======================================================================
Try {
    git clone https://github.com/Hirobreak/libsignal-protocol-c.git
    Set-Location ./libsignal-protocol-c
    git checkout windows
    cmake . -B CptxBuild
    cmake --build CptxBuild --config Release --target install
    Set-Location ..
}
Catch {
    Write-Host $_.Exception.Message`n
}
# =======================================================================
Try {
    git clone https://github.com/civetweb/civetweb.git
    Set-Location ./civetweb
    cmake . -B CptxBuild --loglevel="ERROR"
    cmake --build CptxBuild --config Release --target install
    Set-Location ..
}
Catch {
    Write-Host $_.Exception.Message`n
}
# =======================================================================
Try {
    git clone https://github.com/gabime/spdlog.git
    Set-Location ./spdlog
    cmake . -B build -DCMAKE_BUILD_TYPE=Release --loglevel="ERROR"
    cmake --build build --config Release --target install
    Set-Location ..
}
Catch {
    Write-Host $_.Exception.Message`n
}

# =======================================================================
Try {
    git clone https://github.com/DaveGamble/cJSON.git
    Set-Location ./cJSON
    cmake . -B CptxBuild --loglevel="ERROR"
    cmake --build CptxBuild --config Release --target install
    Set-Location ..
}
Catch {
    Write-Host $_.Exception.Message`n
}
# =======================================================================
Try {
    Set-Location .. 
    Remove-Item deps -Recurse -Force
}
Catch {
    Write-Host $_.Exception.Message`n
}
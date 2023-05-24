shopt -s extglob

rm -rf ./repo
mkdir -p repo
pushd repo
git init
popd

process_version() {
    local filename="$1"; local version="$2"
    local newest_modification_date="`7z -slt l "$filename" | grep Modified | sort -r | head -n1 | cut -d = -f 2`"

    rm -rf repo/*
    7z x "$filename" -orepo -y > /dev/null
    echo "---"
    pushd repo
        if [ -d "openBrf" ]; then
            echo '[i] this release has the files as an extra openBrf folder level; correcting.'
            mv openBrf/* .
            rm -rf openBrf
        fi

        local internal_version="`grep -ohR -E "0\.0\.[0-9]+[ ]*[A-Za-z-]*"  --include \*.cpp`"
        echo "[-] New revision for $filename ($version), internal $internal_version."

        if [ "$version" != "$internal_version" ]; then
            echo "[!] versions don't match ($version vs the internal $internal_version)."
            local extra_text=", internally showing as $internal_version"
        fi
        git add * > /dev/null
        export GIT_COMMITTER_DATE="$newest_modification_date" # https://stackoverflow.com/a/3896112/674685
        git commit -a -m "Version $version$extra_text (from $filename)." --author "Marco Tarini <mtarini@users.noreply.github.com>" --date "$newest_modification_date" > /dev/null
        git tag "$version" > /dev/null
    popd
}

process_version     openBrf_source_0.0.1.zip      0.0.1
process_version     openBrf_source_0.0.4.zip      0.0.4
process_version     openBrf_source_0.0.5.zip      0.0.5
process_version     openBrf_source_0.0.6.zip      0.0.6
process_version     openBrf_source_0.0.7.zip      0.0.7
process_version     openBrf_source_0.0.11.zip     0.0.11
#process_version    openBrf_source_0.0.15.zip     0.0.15   # swy: only 4kb, seems corrupted; skip it
process_version     openBrf_source_0.0.20.zip     0.0.20
process_version     openBrf_source_0.0.33.zip     0.0.33
process_version     openBrf_source_0.0.37.zip     0.0.37
process_version     openBrf_source_0.0.38.zip     0.0.38
process_version     openBrf_source.zip            0.0.39b  # swy: 0.0.39b, see the main_info.cpp file for the about screen
process_version     openBrf_source_0.0.40.zip     0.0.40
process_version     openBrf_source_0.0.41.zip     0.0.41
process_version     openBrf_source_0.0.42.zip     0.0.42
process_version     openBrf_source_0.0.44.zip     0.0.44
process_version     openBrf_source_0.0.45.zip     0.0.45
process_version     openBrf_source_0.0.48.zip     0.0.48
process_version     openBrf_source_0.0.49d.zip    0.0.49d
process_version     openBrf_source_0.0.52d.zip    0.0.52d
process_version     openBrf_source_0.0.53.zip     0.0.53
process_version     openBrf_source_0.0.54b.zip    0.0.54b
process_version     openBrf_source_0.0.55.zip     0.0.55
process_version     openBrf_source_0.0.56.zip     0.0.56
process_version     openBrf_source_0.0.57.zip     0.0.57
process_version     openBrf_source_0.0.58.zip     0.0.58
process_version     openBrf_source_0.0.59.zip     0.0.59
process_version     openBrf_source_0.0.62.zip     0.0.62
process_version     openBrf_source_0.0.63.zip     0.0.63
process_version     openBrf_source_0.0.64.zip     0.0.64
process_version     openBrf_source_0.0.65.zip     0.0.65
process_version     openBrf_source_0.0.66.zip     0.0.66
process_version     openBrf_source_0.0.67.zip     0.0.67
process_version     openBrf_source_0.0.68.zip     0.0.68
process_version     openBrf_source_0.0.69b.zip    0.0.69b
process_version     openBrf_source_0.0.72.zip     0.0.72
process_version     openBrf_source_0.0.74.zip     0.0.74
process_version     openBrf_source_0.0.75.zip     0.0.75
process_version     openBrf_source_0.0.77.zip     0.0.77
process_version     openBrf_source_0.0.78.zip     0.0.78
process_version     openBrf_source_0.0.79.zip     0.0.79
process_version     openBrf_source_0.0.80.zip     0.0.80
process_version     openBrf_source_0.0.82.zip     0.0.82
process_version     openBrf_source_0.0.82b.zip    0.0.82b
process_version     openBrf_source_0.0.82c.zip    0.0.82c
process_version     openBrf_source_0.0.82d.zip    0.0.82d
process_version     openBrf_source_0.0.82e.zip    0.0.82e

git branch -m reconstructed
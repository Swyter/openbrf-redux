shopt -s extglob

process_version() {
    local filename="$1"
    local newest_modification_date="`7z -slt l "$filename" | grep Modified | sort -r | head -n1 | cut -d = -f 2`"

    rm -rf repo/*
    7z x "$filename" -orepo -y > nul

    local version="`grep -R "ver 0.0." repo | head -n 1 | cut -d\> -f 2 | cut -d\< -f 1`"
    #local version="`grep -R "applVersion =" repo | head -n 1 `" #| cut -d\" -f 2`"

    echo " -- $filename ($version)"

    pushd repo
    git commit -a -m "Version $version (from $filename)." --author "Marco Tarini <tarini@isti.cnr.it>" --date "$newest_modification_date"
    git tag "$filename"
    popd
}

process_version     openBrf_source_0.0.1.zip
process_version     openBrf_source_0.0.4.zip
process_version     openBrf_source_0.0.5.zip
process_version     openBrf_source_0.0.6.zip
process_version     openBrf_source_0.0.7.zip
process_version     openBrf_source_0.0.11.zip
process_version     openBrf_source_0.0.15.zip
process_version     openBrf_source_0.0.20.zip
process_version     openBrf_source_0.0.33.zip
process_version     openBrf_source_0.0.37.zip
process_version     openBrf_source_0.0.38.zip
process_version     openBrf_source.zip # swy: 0.0.39b
process_version     openBrf_source_0.0.40.zip
process_version     openBrf_source_0.0.41.zip
process_version     openBrf_source_0.0.42.zip
process_version     openBrf_source_0.0.44.zip
process_version     openBrf_source_0.0.45.zip
process_version     openBrf_source_0.0.48.zip
process_version     openBrf_source_0.0.49d.zip
process_version     openBrf_source_0.0.52d.zip
process_version     openBrf_source_0.0.53.zip
process_version     openBrf_source_0.0.54b.zip
process_version     openBrf_source_0.0.55.zip
process_version     openBrf_source_0.0.56.zip
process_version     openBrf_source_0.0.57.zip
process_version     openBrf_source_0.0.58.zip
process_version     openBrf_source_0.0.59.zip
process_version     openBrf_source_0.0.62.zip
process_version     openBrf_source_0.0.63.zip
process_version     openBrf_source_0.0.64.zip
process_version     openBrf_source_0.0.65.zip
process_version     openBrf_source_0.0.66.zip
process_version     openBrf_source_0.0.67.zip
process_version     openBrf_source_0.0.68.zip
process_version     openBrf_source_0.0.69b.zip
process_version     openBrf_source_0.0.72.zip
process_version     openBrf_source_0.0.74.zip
process_version     openBrf_source_0.0.75.zip
process_version     openBrf_source_0.0.77.zip
process_version     openBrf_source_0.0.78.zip
process_version     openBrf_source_0.0.79.zip
process_version     openBrf_source_0.0.80.zip
process_version     openBrf_source_0.0.82.zip
process_version     openBrf_source_0.0.82b.zip
process_version     openBrf_source_0.0.82c.zip
process_version     openBrf_source_0.0.82d.zip
process_version     openBrf_source_0.0.82e.zip
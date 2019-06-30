bdep init --wipe -C ../KCEngine-release @release cc config.cxx=cl config.cc.coptions=/O2
bdep init -C ../KCEngine-debug @debug cc config.cxx=cl "config.cc.coptions=/MDd /Od /Zi" config.cc.loptions=/DEBUG

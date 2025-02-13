cmd_Release/trill.node := ln -f "Release/obj.target/trill.node" "Release/trill.node" 2>/dev/null || (rm -rf "Release/trill.node" && cp -af "Release/obj.target/trill.node" "Release/trill.node")

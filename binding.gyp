{
  "targets": [{
    "target_name": "trill-addon",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "sources": [ 
      "src/trill-addon.cpp",
      "include/Trill.cpp"
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "include"
    ],
    "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
    "libraries": ["-li2c"]
  }]
}

{
  "targets": [
    {
      "target_name": "trill",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ 
        "src/trill_addon.cpp",
        "lib/Trill.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "lib"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "libraries": [],
      "link_settings": {
        "libraries": [
          "-li2c"
        ]
      }
    }
  ]
}

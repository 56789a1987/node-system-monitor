{
	"targets": [
		{
			"target_name": "sysMonitor",
			"cflags!": [ "-fno-exceptions" ],
			"cflags_cc!": [ "-fno-exceptions" ],
			"sources": [
				"./src/main.cc",
				"./src/cpu.cc",
				"./src/disk.cc",
				"./src/memory.cc",
				"./src/network.cc",
				"./src/sensors.cc",
			],
			"include_dirs": [
				"./src",
				"/usr/include"
			],
			"libraries": [ "-lsensors" ]
		}
	]
}

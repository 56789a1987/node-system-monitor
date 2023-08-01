const sysMonitor = require('../index');

// discard the first result
sysMonitor.getNetworkUsage();

setInterval(() => {
	const networks = sysMonitor.getNetworkUsage();

	for (const { name, recvBytes, transBytes } of networks) {
		console.log(`${name}: download ${(recvBytes / 1024).toFixed(2)} kb/s, upload ${(transBytes / 1024).toFixed(2)} kb/s`);
	}
}, 1000);

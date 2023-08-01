const sysMonitor = require('../index');

// discard the first result
sysMonitor.getCPUUsage();

setInterval(() => {
	const cpus = sysMonitor.getCPUUsage();

	const general = cpus.general;
	const total = general.user + general.nice + general.system + general.idle + general.ioWait + general.irq + general.softIrq;
	const idle = general.idle;

	const individuals = cpus.map(c => {
		const total = c.user + c.nice + c.system + c.idle + c.ioWait + c.irq + c.softIrq;
		const idle = c.idle;
		return ((1 - idle / total) * 100).toFixed(1) + '%';
	});

	console.log(`${((1 - idle / total) * 100).toFixed(1)}% [${individuals.join(' ')}]`);
}, 1000);

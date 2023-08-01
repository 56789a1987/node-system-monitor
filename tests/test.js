const sysMonitor = require('../index');

console.log('Memory', sysMonitor.getMemoryUsage());
console.log('Disk', sysMonitor.getDiskUsage('/'));
console.log('Sensors', JSON.stringify(sysMonitor.getSensors(), undefined, 4));

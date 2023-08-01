declare namespace SysMonitor {
	export interface IMemoryUsage {
		MemTotal: number;
		MemFree: number;
		MemAvailable: number;
		SwapTotal: number;
		SwapFree: number;
	}

	export interface ICPUUsage {
		user: number;
		nice: number;
		system: number;
		idle: number;
		ioWait: number;
		irq: number;
		softIrq: number;
	}

	export interface CPUUsageArray extends ReadonlyArray<ICPUUsage> {
		readonly general: ICPUUsage;
	}

	export interface INetworkUsage {
		name: string;
		recvBytes: number;
		recvPackets: number;
		transBytes: number;
		transPackets: number;
		totalRecvBytes: number;
		totalRecvPackets: number;
		totalTransBytes: number;
		totalTransPackets: number;
	}

	export interface IDiskUsage {
		blockSize: number;
		blocks: number;
		blocksFree: number;
		blocksAvail: number;
	}

	export interface ISensorChipName {
		prefix: string;
		path: string;
		features: ISensorFeature[];
	}

	export interface ISensorFeature {
		name: string;
		number: number;
		type: number;
		subfeatures: ISensorSubfeature[];
	}

	export interface ISensorSubfeature {
		name: string;
		number: number;
		type: number;
		value?: number;
	}

	export function getMemoryUsage(): IMemoryUsage;
	export function getCPUUsage(): CPUUsageArray;
	export function getNetworkUsage(): INetworkUsage[];
	export function getDiskUsage(file: string): IDiskUsage;
	export function getSensors(): ISensorChipName[];
}

export as namespace SysMonitor;
export = SysMonitor;

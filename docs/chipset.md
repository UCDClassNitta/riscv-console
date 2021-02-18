# Console Chipset
The console has a chipset that controls the interrupts and system timer as well as provides access to read the status of the controller buttons. Since the chipset is a custom design, the potential to add capabilities to the chipset may be possible. Below is the register memory location and size. 
| Base Address | Size             | Description                                                                          |
| ------------ | ---------------- | ------------------------------------------------------------------------------------ |
| 0x40000000   | 0x4 (4B)         |  [Interrupt Enable Register](#interrupt-enable-register)                             |
| 0x40000004   | 0x4 (4B)         |  [Interrupt Pending Register](#interrupt-pending-register)                           |
| 0x40000008   | 0x8 (8B)         |  Machine Time (mtime)                                                                |
| 0x40000010   | 0x8 (8B)         |  Machine Time Compare (mtimecmp)                                                     |
| 0x40000018   | 0x4 (4B)         |  [Multi-Button Controller Status Register](#multi-button-controller-status-register) |
| 0x4000001C   | 0x4 (4B)         |  [Cartridge Status Register](#cartidge-status-register)                              |  
| 0x40000020   | 0x4 (4B)         |  [DMA Channel 1 Source Register](#dma-source-register)                               |   
| 0x40000024   | 0x4 (4B)         |  [DMA Channel 1 Destination Register](#dma-destination-register)                     |
| 0x40000028   | 0x4 (4B)         |  [DMA Channel 1 Command Register](#dma-command-register)                             |  
| 0x4000002C   | 0x4 (4B)         |  [DMA Channel 1 Status Register](#dma-status-register)                               |  
| 0x40000030   | 0x4 (4B)         |  [DMA Channel 2 Source Register](#dma-source-register)                               |
| 0x40000034   | 0x4 (4B)         |  [DMA Channel 2 Destination Register](#dma-destination-register)                     |   
| 0x40000038   | 0x4 (4B)         |  [DMA Channel 2 Command Register](#dma-command-register)                             |
| 0x4000003C   | 0x4 (4B)         |  [DMA Channel 2 Status Register](#dma-status-register)                               |
| 0x40000040   | 0x4 (4B)         |  [Machine Clock Period Register](#machine-clock-period-register)                     |
| 0x40000044   | 0x4 (4B)         |  [Video Clock Period Register](#video-clock-period-register)                         |

Notice that the Machine Time and Machine Time Compare registers are 64 bits in size and will require multiple accesses to read or write them. See Machine Timer Registers (mtime and mtimecmp) section of [The RISC-V Instruction Set Manual Volume II: Privileged Architecture](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMFDQC-and-Priv-v1.11/riscv-privileged-20190608.pdf) for more information.

## Interrupt Enable Register
The layout of the Interrupt Enable Register can be seen in the table below. 
| Bit         | 31..6    | 5    | 4    | 3        | 2    | 1   | 0   |
| ----------- | -------- | ---- | ---- | -------- | ---- | --- | --- |
| Description | Reserved | D2IE | D1IE | Reserved | CMIE | VIE | CIE |

The CIE bit represents the Cartridge Interrupt Enable bit, the VIE bit represents the Video Interrupt Enable bit, the CMIE bit represents the Command Interrupt Enable bit, and the D1IE and D2IE bits represent the DMA channel 1 and 2 Interrupt Enable bits respectively. The CIE, VIE, CMIE, D1IE, and D2IE have corresponding Pending Interrupt bits CIP, VIP, CMIP, D1IP, and D2IP in the [Interrupt Pending Register](#interrupt-pending-register). 

## Interrupt Pending Register
See table below for the layout of the Interrupt Pending Register. The pending interrupt is cleared by writing a 1 to the corresponding bit, **NOT** by writing a zero. The reason for writing a 1 is that the register doesn't need to be read ANDed and then written back in order to clear the pending interrupt. 
| Bit         | 31..6    | 5    | 4    | 3        | 2    | 1   | 0   |
| ----------- | -------- | ---- | ---- | -------- | ---- | --- | --- |
| Description | Reserved | D2IP | D1IP | Reserved | CMIP | VIP | CIP |

## Multi-Button Controller Status Register
The Multi-Button Controller Status Register layout can be seen below. 
| Bit         | 31..8    | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
| ----------- | -------- | --- | --- | --- | --- | --- | --- | --- | --- |
| Description | Reserved | B4D | B3D | B2D | B1D | RDD | DDD | UDD | LDD |

The LDD, UDD, DDD, and RDD bit represent the status of the Left Direction, Up Direction, Down Direction and Right Direction switches if they are Depressed. The buttons 1 – 4 are represented by the B1D through B4D bits.


## Cartridge Status Register
The Cartridge Status Register layout can be seen in below. 
| Bit         | 31..2         | 1        | 0  |
| ----------- | ------------- | -------- | --- |
| Description | Entry Address | Reserved | CIS |

The CIS bit specifies if the cartridge is inserted or not. The Entry Address is the entry point to the cartridge application. It is assume that instructions are 32-bit aligned in RISC-V 32, so the low two bits will be zero for the Entry Address.

## DMA Channel
The console chipset has two integrated DMA channels that are each capable of transfering up to 16,777,215 bytes in a transfer. Once intiated the DMA channel will transfer up to 32-bits per CPU cycle until the transfer has been completed.

### DMA Source Register
The DMA Source Register layout can be seen below. Each source register is a memory source base for the beginning of the DMA transfer. 
| Bit         | 31..0          |
| ----------- | -------------- |
| Description | Source Address |


### DMA Destination Register
The DMA Destination Register layout can be seen below. Each destination register is the memory destination base for the beginning of the DMA transfer.
| Bit         | 31..0               |
| ----------- | ------------------- |
| Description | Destination Address |

### DMA Command Register
The DMA Command Register layout can be seen below. The SDT bit specifies to start a DMA transfer, it is always read as a zero. The Transfer Size specifies the number of bytes to transfer from the source to destination, if zero bytes are specified the current transfer will be cancelled. 
| Bit         | 31  | 30..24   | 23..0         |
| ----------- | --- | -------- | ------------- |
| Description | SDT | Reserved | Transfer Size |


### DMA Status Register
The layout of the DMA Status Register can be seen below. The DTA bit specifies if a DMA transfer is active currently and if commands will be ignored. The SME and DME specify if a source or destination memory error occurred on the previous DMA transfer. The DTC bit specifies if the previous DMA transfer was cancelled. The SME, DME, and DTC bits will be cleared upon initialization of the next DMA transfer.
| Bit         | 31  | 30  | 29  | 28  | 27..24   | 23..0              |
| ----------- | --- | --- | --- | --- | -------- | ------------------ |
| Description | DTA | SME | DME | DTC | Reserved | Transfer Remaining |

## Machine Clock Period Register
The Machine Clock Period Register provides the clock period of the Machine Time clock in microseconds. The register is read only.
| Bit         | 31..0  |
| ----------- | ------ |
| Description | Period |

## Video Clock Period Register
The Video Clock Period Register provides the clock period of the Video Timer Tick clock in milliseconds. The register is read only.
| Bit         | 31..0  |
| ----------- | ------ |
| Description | Period |

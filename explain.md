## Global Descriptor Table
The Global Descriptor Table (GDT) is a data structure used in x86 and x86-64 architectures to manage memory segmentation and access control. It is an integral part of memory protection and privilege levels in these architectures.

### Layout in memory:
----------------------------------------------------
|         Field         |   Size (bits)   | Offset |
|:---------------------:|:---------------:|:------:|
|         Limit         |       16        |   0    |
|         Base          |       16        |   16   |
|         Base          |        8        |   32   |
|        Present        |        1        |   40   |
|       Privilege       |        2        |   41   |
|         Type          |        1        |   43   |
|         Code          |        1        |   44   |
|  Conforming/direction |        1        |   45   |
|   Readable/Writable   |        1        |   46   |
|       Accessed        |        1        |   47   |
|      Granularity      |        1        |   48   |
|        32-bit         |        1        |   49   |
|        64-bit         |        1        |   50   |
|         AVL           |        1        |   51   |
|         Base          |        8        |   52   |
----------------------------------------------------

### Short version:
| Field                | Size (bits) | Description                                                                       |
|----------------------|-------------|-----------------------------------------------------------------------------------|
| Base                 | 32          | The starting memory location of the segment (pointer to the beginning).          |
| Limit                | 20          | Defines the size of the segment in bytes (maximum offset within the segment).    |
| Present              | 1           | Indicates whether the segment is in use (0b1 for present, 0b0 for not in use).   |
| Privilege            | 2           | Determines the privilege level of the segment (0b00 for highest, 0b11 for lowest). |
| Type                 | 1           | Specifies if it's a code (0b1) or data (0b0) segment.                             |
| Code                 | 1           | For code segments, indicates executability (0b1 for executable, 0b0 otherwise).   |
| Conforming/Direction | 1           | For code segments, determines whether lower privilege levels can execute (0b1) or not (0b0). For data segments, can indicate segment growth direction. |
| Readable/Writable    | 1           | For code segments, specifies readability (0b1 for readable, 0b0 otherwise). For data segments, indicates writability (0b1 for writable, 0b0 otherwise). |
| Accessed             | 1           | Managed by CPU, set to 1 when the segment is accessed.                           |
| Granularity          | 1           | If 0b1, the limit is multiplied by 0x1000 (4096), effectively giving a larger addressable range. |
| 32-bit               | 1           | Indicates 32-bit mode (0b1) or not (0b0).                                        |
| 64-bit               | 1           | Used in x86-64 architecture, indicates 64-bit mode (0b1) or 32-bit compatibility mode (0b0). |
| AVL                  | 1           | Available for Use by Software, can be used for custom purposes by software or OS.  |

### Longer version:

1. Base (4 bytes | 32 bits)
   * The base address represents the starting memory location of the segment.
   * It serves as a pointer to the beginning of the segment.

2. Limit (2.5 bytes | 20 bits)
   * The limit defines the size of the segment in bytes.
   * It specifies the maximum offset within the segment.

3. Present (1 bit)
   * This bit indicates whether the segment is currently in use (present) or not.
   * 0b1 signifies that the segment is present and active, while 0b0 indicates it's not in use.

4. Privilege (2 bit)
   * These bits determine the privilege level of the segment.
   * Privilege levels range from 0 (most privileged) to 3 (least privileged).
   * 0b00 is typically used for the highest privilege level (kernel mode), and 0b11 for the lowest (user mode).

5. Type (1 bit)
   * This bit specifies the type of the segment, whether it's a code segment or a data segment.
   * 0b1 indicates a code segment, and 0b0 indicates a data segment.

6. Type flags
    * Code: This flag indicates whether the segment is executable or not.
        * 0b1 signifies that the segment contains executable code.
        * 0b0 indicates that it's not executable.
    * Conforming/Direction
      * Code = 0b0 Direction: Used to indicate whether the segment grows upwards (Direction bit set to 0) or downwards (Direction bit set to 1) in memory.
      * Code = 0b1 Conforming: Relevant only for code segments, this flag determines whether the segment can be used by lower privilege levels.
        * 0b1 means that lower privilege levels can execute code in this segment.
        * 0b0 restricts access to higher privilege levels only.
    * Readable/Writable 
      * Code = 0b0 Writable: Set's wether the segment is writable.
        * 0b1 indicates that the segment is writable.
        * 0b0 means it's readonly.
      * Code = 0b1 Readable: For code segments, this flag specifies whether the segment contains readable data (e.g., constants).
        * 0b1 indicates that the segment is readable.
        * 0b0 means it's not readable.
    * Accessed: This flag is managed by the CPU and is set to 1 when the segment is accessed, allowing the system to monitor segment usage.

7. Other flags
    * Granularity: If set to 0b1, the limit is multiplied by 0x1000 (4096), effectively giving a larger addressable range. This is often used to switch between real mode and protected mode in x86 architecture.
    * 32-bit: When set to 0b1, it indicates that the segment operates in 32-bit mode. This flag is relevant in the x86-64 architecture, where it allows for 32-bit compatibility mode.
    * 64-bit (1 bit):
        * This bit is used in the x86-64 architecture to indicate whether the segment is in 64-bit mode or not.
        * When set to 0, it means the segment operates in 32-bit compatibility mode.
        * When set to 1, it indicates that the segment is intended for 64-bit mode.

    * AVL (1 bit):
        * The AVL (Available for Use by Software) bit is reserved for software use.
        * It doesn't have a specific predefined purpose in the GDT itself and can be used by software or operating systems for custom purposes.
        * It's often used for additional flags or information that might be needed by the operating system or specific software components.
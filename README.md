#3Pi Tracking

##GUI App Controls
* Use you mouse to select bots
* Press ' '<Space> to capture a new background
* Press 'g' —for GUI— to open the menu for all sorts of goodies

##Messageing Protocol

###Byte legend
| Byte Var  | Definition         |
| ---------:|:------------------ |
| i         | Client Message ID  |
| r         | Robot ID           |
| R         | Robot Radius       |
| n         | number of Robots   |
| t         | Time Stamp byte    |
| x         | Robot X Coord byte |
| y         | Robot Y Coord byte |
| z         | Robot Z Coord byte |


###Client Outgoing Message Reference
| Number | Definition       | Byte Sequence |
| ------:|:---------------- |:------------- |
| 0      | New Background   | i             |
| 1      | Toggle Tracking  | i             |
| 2      | Add Robot        | ir            |
| 3      | World Dimensions | i             |
| 4      | Robot Positions  | i             |

###Server Outgoing Message Reference
| Number | Definition       | Byte Sequence       |
| ------:|:---------------- |:------------------- |
| 0      | New Background   | i                   |
| 1      | Toggle Tracking  | i                   |
| 2      | Add Robot        | irxxyyzzR           |
| 3      | World Dimensions | iwwhh               |
| 4      | Robot Positions  | intttt{rxxyyzz * n} |

####Details
   * 6 + 7 * n

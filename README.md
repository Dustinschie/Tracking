#3Pi Tracking

##App Controls
* Use you mouse to select bots
* Press ' '<Space> to capture a new background
* Press 'G' to open the menu for all sorts of goodies


Byte LEGEND
–––––––––––
  i:  message indicator
  r:  robot ID
  R:  robot radius
  n:  number of robots
  t:  timestamp byte
  x:  x byte
  y:  y byte
  z:  z byte

MESSAGE INFORMATION FROM CLIENT
–––––––––––––––––––––––––––––––
0:  NEW BG                => i

1:  START TRACKING        => i

2:  ADD BOT:  2           => ir

3:  Dim                   => i

4:  Cur_pos               => i


MESSAGE INFORMATION FROM SERVER
–––––––––––––––––––––––––––––––
  0:  NEW BG                => i

  1:  START TRACKING        => i

  2:  ADD BOT:  9           => irxxyyzzR
      id:       1 bytes
      Robo_ID:  1 bytes
      x:        2 bytes
      y:        2 bytes
      z:        2 bytes
      rad:      1 bytes

  3:  Dim:      5           => iwwhh
      id:       1 bytes
      width:    2 bytes
      height:   2 bytes

  4:  cur_Pos:  alot        => intttt{rxxyyzz * n} = 6 + 7 * n
      id:       1 bytes
      timestamp:4 bytes
      loop
       robo_id: 1 bytes
       x:       2 bytes
       y:       2 btyes
       z:       2 bytes
      endLoop

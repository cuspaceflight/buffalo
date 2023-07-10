Launch of Martlet 3 at BALLS in October 2017.

M3 datalogging format:
u16 ID, u8 RTR, u8 length, 8B data, u32 timestamp (in 1/1e4 seconds)
ID is (48<<5)|1 for accel and (49<<5)|1 for baro.
Accel is i16 [x y z], baro is i32 [temperature, pressure]

Booster data is from around 1926 seconds to end, around 1935.
Dart data is from around 1940 onwards.

# common serial port UART bps rates 19.2k, 38.4k, 57.6k, 115.2k, 230.4k, 460.8k
bps =230400*2 #bits transmitted per second
fcpu =14745600 # [ECS-147.4-20-3X-TR] can do 460.8k baud but the avr should have a 3.55V supply.
BAUD_SETTING= int( ((fcpu + bps * 4) / (bps * 8)) - 1 ) 
BAUD_ACTUAL=( (fcpu/(8 * (BAUD_SETTING+1))) )
BAUD_ERROR=( (( 100*(bps - BAUD_ACTUAL) ) / bps) )
print("BPS_SETTING:%i  ACTUAL:%f  ERROR:%f" % (BAUD_SETTING, BAUD_ACTUAL, BAUD_ERROR))
# note an error of up to 2% is normally acceptable, anything over 3% is a problem. 
input("Any key won't work you have to Enter to exit, a deep thought ;)" ) 

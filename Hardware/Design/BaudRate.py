# common serial port UART bps rates 19.2k, 38.4k, 57.6k, 115.2k, 230.4k, 460.8k
# fcpu = 14745600 # [ECS-147.4-20-3X-TR] can do 460.8k baud but the avr should have a 3.55V supply
# fcpu = 16000000 # can do 38.4k, 76.4k, and 152.8k with less than .2% error, picocom only has 38.4k.
# fcpu = 16000000 # can do 125k, 250k, 500k with 0% error, Works on Python and Linux, but not picocom (it was fixed but not yet released https://github.com/npat-efault/picocom/releases).
bps =250000*2 #bits transmitted per second
fcpu =16000000
BAUD_SETTING= int( ((fcpu + bps * 4) / (bps * 8)) - 1 ) 
BAUD_ACTUAL=( (fcpu/(8 * (BAUD_SETTING+1))) )
BAUD_ERROR=( (( 100*(bps - BAUD_ACTUAL) ) / bps) )
print("BPS_SETTING:%i  ACTUAL:%f  ERROR:%f" % (BAUD_SETTING, BAUD_ACTUAL, BAUD_ERROR))
# note an error of up to 2% is normally acceptable, anything over 3% is a problem. 
input("Any key won't work you have to Enter to exit, a deep thought ;)" ) 

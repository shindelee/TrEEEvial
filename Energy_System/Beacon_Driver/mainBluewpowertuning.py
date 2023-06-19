from machine import Pin, ADC, PWM
import time

vret_pin = ADC(Pin(26))
vout_pin = ADC(Pin(28))
vin_pin = ADC(Pin(27))
pwm = PWM(Pin(0))
freq = 180000
pwm.freq(freq)
pwm_en = Pin(1, Pin.OUT)
pwm_out = 0
alpha = 0.1 # exponential filter smoothing factor

def EXPfilter(signal):
    prev_output = 0
    filtered_signal = alpha*signal + (1-alpha)*prev_output
    return filtered_signal

def saturate(duty):
    if duty > 62500:
        duty = 62500
    if duty < 100:
        duty = 100
    return duty

def ADC_to_voltage(voltage):
    voltage = (3.3*voltage)/65536
    return voltage

def Vin_correction(voltage): # applies error correction through extensive method
    voltage = ((ADC_to_voltage(voltage)/(0.2*0.968))-0.066)/0.9911
    return voltage

def Vout_correction(voltage):
    voltage = ((abs((ADC_to_voltage(voltage)/0.2)-0.12771)/1.0354)+0.0118)/1.0672
    return voltage

def ADC_to_current(voltage):
    current = ((ADC_to_voltage(voltage)/1.0227)-0.009)/1.003
    return current

def Duty_conv(duty):
    duty = 100*duty/62500
    return duty

def saturation(parameter, low_lim, up_lim):
    if parameter > up_lim:
        parameter = up_lim
    if parameter < low_lim:
        parameter = low_lim
    return parameter

class PIDController:
    def __init__(self, kp, ki, kd, up_lim, low_lim):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.up_lim = up_lim
        self.low_lim = low_lim
        self.error = 0
        self.prev_error = 0
        self.int = 0
        self.der = 0
        self.last_time = 0
        self.dt = 0
    def controller(self, parameter, reference):
        cur_time = time.ticks_ms()
        self.dt = (cur_time - self.last_time)/1000
        print("dt{:.2f}".format(self.dt),end='	')
        self.error = reference - parameter
        print("error{:.2f}".format(self.error),end='	')
        self.int += self.error*self.dt
        # integral windup prevention
        
        # method: clamping - bounds the integration to saturation values
        if self.int >= self.up_lim or self.int <= self.low_lim:
            self.int = 0
        self.int = EXPfilter(self.int)
        self.int = abs(self.int - reference)
        
        print("int{:.2f}".format(self.int),end='	')
        self.der = (self.error-self.prev_error)/self.dt if self.dt>0 else 0
        control_signal = (self.kp*self.error)+(self.ki*self.int)+(self.kd*self.der)
        self.prev_error = self.error
        self.last_time = cur_time
        return control_signal

vkp = 0.84
vki =13.25
vkd = 0

ikp = 0.07
iki = 1.7
ikd = 0

vup_lim = 5
vlow_lim = 0
iup_lim = 50
ilow_lim = 0

vref = 2.35
imax = 40 # milliamps
vPID = PIDController(vkp, vki, vkd, vup_lim, vlow_lim)
iPID = PIDController(ikp, iki, ikd, iup_lim, ilow_lim)

while True:
    pwm_en.value(1)
    vin = Vin_correction(vin_pin.read_u16())
    vout = Vout_correction(vout_pin.read_u16())
    iout = ADC_to_current(vret_pin.read_u16())
    iout_mA = iout*1000
    
    
    iref = vPID.controller(vout, vref) # produce a control signal that has a value between 0 and 50
    print("iref{:.2f}".format(iref),end='	')
    iref = saturation(iref, 0, imax)
    print("irefsat{:.2f}".format(iref),end='	')
    
    duty_ref = iPID.controller(iout_mA, iref) # produce a duty cycle that has a value between 1 and 99 (duty cycle precentages)
    
    # PID for stability, conditional statements for power tunings at various input voltages
    
    if vin <= 6.5 and vin > 5.5:
        duty_ref += 5
        pwm.freq(freq - 17500)
    elif vin <= 5.5 and vin > 4.75:
        duty_ref += 10
        pwm.freq(freq - 20000)
    elif vin <=4.76 and vin > 4:
        duty_ref += 22.5
        pwm.freq(freq  - 45000)
    elif vin <= 4 and vin > 3.8:
        duty_ref += 30
        pwm.freq(freq - 60000)
    elif vin <= 3.8:
        duty_ref += 40
        pwm.freq(freq - 80000)
    elif vin <= 9 and vin > 8:
        duty_ref -=4
        pwm.freq(freq + 7500)
    elif vin <= 10 and vin > 9:
        duty_ref -= 6
        pwm.freq(freq + 8500)
    elif vin <= 11 and vin > 10:
        duty_ref -=8
        pwm.freq(freq + 15000)
    elif vin <= 12 and vin > 11:
        duty_ref -=12.5
        pwm.freq(freq + 19500)
    elif vin <= 13 and vin > 12:
        duty_ref -= 15
        pwm.freq(freq + 22500)
    elif vin <= 14 and vin > 13: 
        duty_ref -=15
        pwm.freq(freq + 25000)
    else:
        pwm.freq(freq)
    
    print("duty{:.2f}".format(duty_ref),end='	')
    duty_ref *=625
    print("duty{:.2f}".format(duty_ref),end='	')

    
    pwm_duty = duty_ref
    
    
    pwm_out = saturation(pwm_duty, 100, 62500) # limits the duty cycle to 0.01 and 0.99
    pwm.duty_u16(int(pwm_out))

    # DIAGNOSTIC TEXT
    print("Vin = {:.2f}".format(vin) + " V" , end='	')
    print("Vout = {:.2f}".format(vout)+ " V", end='	')
    print("Iout = {:.3f}".format(iout) + " A", end='	')
    print("Duty = {:.2f}".format(Duty_conv(pwm_out))+' %')
    
    time.sleep_ms(167)




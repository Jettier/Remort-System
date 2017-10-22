import RPi.GPIO as GPIO
from time import sleep
print(sleep)


GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(4,GPIO.IN)
GPIO.setup(5,GPIO.IN)
GPIO.setup(6,GPIO.IN)
GPIO.setup(7,GPIO.IN)
GPIO.setup(8,GPIO.IN)
GPIO.setup(9,GPIO.IN)
GPIO.setup(10,GPIO.IN)
GPIO.setup(11,GPIO.IN)

GPIO.setup(12,GPIO.IN)
GPIO.setup(13,GPIO.IN)
GPIO.setup(14,GPIO.IN)
GPIO.setup(15,GPIO.IN)
GPIO.setup(16,GPIO.IN)
GPIO.setup(17,GPIO.IN)
GPIO.setup(18,GPIO.IN)
GPIO.setup(19,GPIO.IN)
GPIO.setup(20,GPIO.IN)
GPIO.setup(21,GPIO.IN)
GPIO.setup(26,GPIO.IN)

chk = GPIO.input(26)
print(chk)
while chk == GPIO.LOW:
    word = ""

    for port in range(4,11):
        b = GPIO.input(port)
        if (b == GPIO.HIGH):
            word = word + "O"
        else:
            word = word +"X"
    word = word + "_"

    for port in range(12,21):
        b = GPIO.input(port)
        if (b == GPIO.HIGH):
            word = word + "O"
        else:
            word = word +"X"


    print( word)
    sleep(0.05)

        
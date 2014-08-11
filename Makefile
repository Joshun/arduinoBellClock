CC = gcc
INSTALLDIR = /opt/arduinoBellClock

all: clock

clock: config.o arduino.o clock.c
	$(CC) -o $@ $^

arduino.o: arduino.c
	$(CC) -c $<

config.o: config.c
	$(CC) -c $<

clean:
	rm -rf *.o clock

install:
	-mkdir /opt
	mkdir $(INSTALLDIR)
	mkdir $(INSTALLDIR)/src
	cp clock $(INSTALLDIR)
	cp clock_wrapper.sh $(INSTALLDIR)
	cp clock.cfg $(INSTALLDIR)
	cp *.c *.h $(INSTALLDIR)/src
	cp -r arduino $(INSTALLDIR)/src
	ln -s $(INSTALLDIR)/clock_wrapper.sh /usr/bin/arduinoBellClock
	ln -s $(INSTALLDIR)/clock.cfg /etc/arduinoBellClock.cfg
	ln -s $(INSTALLDIR)/clock.log /var/log/arduinoBellClock.log

uninstall:
	rm -rf $(INSTALLDIR)
	rm -f /usr/bin/arduinoBellClock
	rm -f /etc/arduinoBellClock.cfg
	rm -f /var/log/arduinoBellClock.log

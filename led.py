#!/usr/bin/python
import argparse
import os

def arg_parser():
	parser = argparse.ArgumentParser()
	parser.add_argument("-r", "--red", nargs='?', const='255', type=int, help="Red value")
	parser.add_argument("-g", "--green", nargs='?', const='255', type=int, help="Green value")
	parser.add_argument("-b", "--blue", nargs='?', const='255', type=int, help="Blue value")
	parser.add_argument("-q", "--quiet", action='store_true')
	parser.add_argument("-fire", dest='fire', action='store_true',  help="Set fire mode")
	parser.set_defaults(fire=False)
	parser.add_argument("-ocean", dest='ocean', action='store_true',  help="Set ocean mode")
	parser.set_defaults(ocean=False)
	parser.add_argument("-forest", dest='forest', action='store_true',  help="Set forest mode")
	parser.set_defaults(forest=False)
	parser.add_argument("-p", "--port", help="Arduino port")
	parser.add_argument('-xD', dest='xD', action='store_true', help='xD')
	parser.set_defaults(xD=False)

	return parser.parse_args()

def sendColor(r, g, b, port):
	color = ','.join(str(x) for x in [r, g, b])
	cmd ="echo " + color + " > " + port
	os.system(cmd)
	


def main(args):
	red = args.red if args.red else 0
	green = args.green if args.green else 0
	blue = args.blue if args.blue else 0
	
	port = "/dev/ttyACM" + (args.port if args.port else "0")

	os.system("stty -F " + port + " cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts")

	if (args.fire):
		if (not args.quiet):
			print "Setting mode: Fire!"
		sendColor(-1, 0, 0, port)
	elif (args.ocean):
		if (not args.quiet):
			print "Setting mode: Ocean!"
		sendColor(0, -1, 0, port)
	elif (args.forest):
		if (not args.quiet):
			print "Setting mode: Forest!"
		sendColor(0, 0, -1, port)
	elif (args.xD):
		if (not args.quiet):
			print 'xD'
		sendColor(-1, -1, 0, port)
	else:
		if (not args.quiet):
			print "Setting color: " + ','.join(str(x) for x in [red, green, blue])
		sendColor(red, green, blue, port)
		
main(arg_parser())

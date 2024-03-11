import argparse

parser = argparse.ArgumentParser(prog="PROG", 
                                description='Process some integers.',
                                usage='%(prog)s [options]',
                                epilog="This is end of help",)
parser.add_argument('integers', metavar="N", type=int, nargs='+',
                    help='an integer for the accumulator')
parser.add_argument('--sum', dest='accumulate', action='store_const',
                    const=sum, default=max,
                    help='sum the integers (default: find the max)')

args = parser.parse_args()
print(args.accumulate(args.integers))   

# parser = argparse.ArgumentParser(description="mancibo cibo")
# parser.add_argument("ucup", type=int, help="ucup surucup")
# parser.add_argument("otong", type=int, help="otong surotong ")
# args = parser.parse_args()

# def tast(a,b):
#     x = a*b
#     return x

# if __name__ == '__main__':
#     print (tast(args.ucup, args.otong))

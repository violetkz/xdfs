#!/usr/bin/python
import requests
import time

from multiprocessing import Pool

def send_req(url):
    for a in xrange(10):
        r = requests.get(url)


if __name__ == '__main__':
    import sys
    if len(sys.argv) >=2:
        p = Pool(10)
        for x in xrange(10):
            #print(p.apply(send_req, [sys.argv[1] for x in xrange(100)]))
            p.apply_async(send_req, (sys.argv[1],))
        p.close()
        p.join()

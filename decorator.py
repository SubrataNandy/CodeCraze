def deco1(depends_on):
    print "inside deco1"
    depends_on()
    def deco2(orig_func):
        print "inside deco2"
        def new_func(*args, **kvargs):
            print "before orig func call"
            ret = orig_func(*args, **kvargs)
            return ret
        return new_func
    return deco2

def depends_on(*args, **kvargs):
    print "in depend_on"
    return

# @deco1(depends_on)
# def func(val):
#     print val
#     return func

# func(420)


### Below is same as above commented section

def func(val):
    print val
    return func

d1 = deco1(depends_on)
d2 = d1(func)
d2(420)

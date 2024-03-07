def linearFunction(x1, y1, x2, y2) :
    m = (y2-y1) / (x2-x1)
    b = -(m*x1 -y1)
    return m, b
f = open("code.txt", "w+") 


# nums = [[990, 10.8], [957, 9.1], [926, 8.2], [900, 7.6], [877,7], [857, 6.7], [801, 5.9], [741, 5.4], [712, 4.9], [0, 0]]
nums = [[306, 2.2], [325, 2.3], [365, 2.4], [371, 2.4], [396, 2.5], [397, 2.6], [401, 2.8], [421, 3], [457, 3.1], [497, 3.2], [537, 3.3], [560, 3.4], [583, 3.7], [622, 3.9], [688, 4.3], [742, 4.8], [787, 5.7], [847, 6.5], [901, 7.5], [956, 9.2], [985, 10.2]]
for x in range(0, len(nums)-1):
    fun = linearFunction(nums[x][0], nums[x][1], nums[x+1][0], nums[x+1][1])
    m = fun[0]
    b = fun[1]
    f.write("if (analogLight < %s) { \n\t\tm = %s; \n\t\tb = %s; \n\t} else " % (nums[x+1][0], m, b))
    print(m, b)
f.close()




# def thingForFermat(x):
#     x1 = 1 / x
#     x2 = 1 / (x+2)
#     return x1-x2
# sum=0
# for x in range(1, 2001):
#     sum += thingForFermat(x)
#     print("x:  ", x,  thingForFermat(x),  sum)
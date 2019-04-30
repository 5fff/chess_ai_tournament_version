for rank in [1,2,3,4,5,6,7,8]:
    for file in 'abcdefgh':
        print('"'+file+str(rank)+'",',end = "")
    print("\n",end="")


# reverse map table

index = 0

for rank in [1,2,3,4,5,6,7,8]:
    for file in 'abcdefgh':
        print('{"'+file+str(rank)+'",',index,'},',end = "\t")
        index += 1
    print("\n",end="")

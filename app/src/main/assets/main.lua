

function add(a,b)
    return a + b
end

-- 加载Lua脚本时，就会执行
print('c_add的结果：'..c_add(3,4))
require "ifj21"
function main(y : number) : integer, integer
    local x : integer = 10
    x, x = main(x)
    main(x)
    return 20, 20
end
main(20)

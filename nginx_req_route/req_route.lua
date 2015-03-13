
-- 深圳
OUT_CODE = 433
-- 合作区
IN_CODE = 434


-- 走合作区的 pf: locs
IN_PARAMS = {
    qzone = {
        '/v3/user/get_info',
        '/req_route',
    },
}

-- 走合作区的appid列表
IN_APPIDS = {
    600,
}


-------------------------------------------------------
-- 参数:待分割的字符串,分割字符
-- 返回:子串表.(含有空串)
function string_split(str, split_char)
    local sub_str_tab = {};
    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            sub_str_tab[#sub_str_tab + 1] = str;
            break;
        end
        local sub_str = string.sub(str, 1, pos - 1);
        sub_str_tab[#sub_str_tab + 1] = sub_str;
        str = string.sub(str, pos + 1, #str);
    end

    return sub_str_tab;
end

function parse_params(method, args, body)
    --选择从get参数还是post参数中获取
    if method == 2 then 
        data = args
    elseif method == 8 then 
        data = body
    else 
        return nil
    end

    --解析对应的params
    params = {}
    pair_list = string_split(data, '&')
    for k, v in pairs(pair_list) do
        t = string_split(v, '=')
        if #t == 2 then
            params[t[1]] = t[2]
        end
    end

    return params
end

function is_match_pf_and_loc(pf, loc)
    loc_list = IN_PARAMS[pf]

    if loc_list == nil then
        return false
    end

    for k, v in pairs(loc_list) do
        if v == loc then
            return true
        end
    end

    return false
end

function is_match_appid(appid)
    for k, v in pairs(IN_APPIDS) do
        if v == appid then
            return true
        end
    end

    return false
end

--method: get:2, post:8
--uri: /req_route
--args: x=1
--body: y=1
--return: 433: 深圳, 434:合作区

function get_route_id(method, uri, args, body)

    loc = uri
    params = parse_params(method, args, body)

    if loc == nil or params == nil or params['pf'] == nil or params['appid'] == nil then
        return OUT_CODE
    end

    pf = params['pf']
    appid = tonumber(params['appid'])

    --到这里位置，就把所有的数据都拿到了
    --print (loc, pf, appid)


    -- 找是否在对应的url, loc中
    if not is_match_pf_and_loc(pf, loc) then
        return OUT_CODE
    end

    -- 找是否在对应的appid中
    if not is_match_appid(appid) then
        return OUT_CODE
    end

    return IN_CODE
end


function main()
    method = 8
    uri = '/req_route'
    args = nil
    body = 'pf=qzone&appid=600&&openid=1'

    route_id = get_route_id(method, uri, args, body)

    print (route_id)
end

--测试用的
main()

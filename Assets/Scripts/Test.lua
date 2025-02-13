--
-- > Notice: Amélie Heinrich @ 2025
-- > Create Time: 2025-02-13 15:03:27
--

return function(entityID)
    local self = {}

    function self.awake()
        print("FindPlayer.lua Awake for Entity " .. entityID)
    end

    function self.update(dt)
        print("FindPlayer.lua Update for Entity " .. entityID)
    end

    function self.quit()
        print("FindPlayer.lua Quit for Entity " .. entityID)
    end

    return self
end

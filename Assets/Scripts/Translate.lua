--
-- > Notice: Amélie Heinrich @ 2025
-- > Create Time: 2025-02-13 19:03:26
--

return function(entityID)
    local self = {}

    function self.awake()
        print("hello")
        self.transform = Entity.GetTransform(entityID)
    end

    function self.update(dt)
        if Input.IsKeyDown(Keycode.Left) then
            self.transform.position.z = self.transform.position.z - (1.0 * dt)
        end
        if Input.IsKeyDown(Keycode.Right) then
            self.transform.position.z = self.transform.position.z + (1.0 * dt)
        end
        if Input.IsKeyDown(Keycode.Up) then
            self.transform.position.x = self.transform.position.x + (1.0 * dt)
        end
        if Input.IsKeyDown(Keycode.Down) then
            self.transform.position.x = self.transform.position.x - (1.0 * dt)
        end
    end

    function self.quit()
        print("hello")
    end

    return self
end

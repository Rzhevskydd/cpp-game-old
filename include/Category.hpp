#ifndef BOOK_CATEGORY_HPP
#define BOOK_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category {
        enum Type : unsigned {
                None              = 0,
                Scene             = 1 << 0,
                Player            = 1 << 1,
                Allied            = 1 << 2,
                Enemy             = 1 << 3,
                Pickup            = 1 << 4,
                AlliedProjectile  = 1 << 5,
                EnemyProjectile   = 1 << 6,
                TileLayout        = 1 << 7,
                LevelObject       = 1 << 8,
                BoundingBox       = 1 << 9,
                LevelObjectLayout = 1 << 10,
                BoundingBoxLayout = 1 << 11,
                Foreground        = 1 << 12,
                Projectile        = AlliedProjectile | EnemyProjectile,
        };
}

#endif  // BOOK_CATEGORY_HPP

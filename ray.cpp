#include <cmath>
#include <iostream>
#include <vector>

struct vec3 {
    vec3() = default;

    vec3(float e0, float e1, float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    inline float x() const noexcept { return e[0]; }

    inline float y() const noexcept { return e[1]; }

    inline float z() const noexcept { return e[2]; }

    inline float r() const noexcept { return e[0]; }

    inline float g() const noexcept { return e[1]; }

    inline float b() const noexcept { return e[2]; }

    inline const vec3& operator+() const noexcept { return *this; }

    inline vec3 operator-() const noexcept { return vec3(-e[0], -e[1], -e[2]); }

    inline float operator[](int i) const noexcept { return e[i]; }

    inline float& operator[](int i) noexcept { return e[i]; }

    inline vec3& operator+=(const vec3& v2) noexcept {
        e[0] += v2[0];
        e[1] += v2[1];
        e[2] += v2[2];
        return *this;
    }

    inline vec3& operator-=(const vec3& v2) noexcept {
        e[0] -= v2[0];
        e[1] -= v2[1];
        e[2] -= v2[2];
        return *this;
    }

    inline vec3& operator*=(const vec3& v2) noexcept {
        e[0] *= v2[0];
        e[1] *= v2[1];
        e[2] *= v2[2];
        return *this;
    }

    inline vec3& operator/=(const vec3& v2) noexcept {
        e[0] /= v2[0];
        e[1] /= v2[1];
        e[2] /= v2[2];
        return *this;
    }

    inline vec3& operator*=(const float t) noexcept {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    inline vec3& operator/=(const float t) noexcept {
        float k = 1.0 / t;

        e[0] *= k;
        e[1] *= k;
        e[2] *= k;

        return *this;
    }

    inline float length() const noexcept {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline float squared_length() const noexcept {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    void make_unit_vector() noexcept {
        float k  = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
        e[0]    *= k;
        e[1]    *= k;
        e[2]    *= k;
    }

    float e[3];
};

inline std::istream& operator>>(std::istream& is, vec3& t) {
    return is >> t.e[0] >> t.e[1] >> t.e[2];
}

inline std::ostream& operator<<(std::ostream& os, const vec3& t) {
    return os << t.e[0] << t.e[1] << t.e[2];
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
    return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
    return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 operator/(const vec3& v1, const vec3& v2) {
    return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline vec3 operator/(vec3 v, float t) {
    return vec3(v[0] / t, v[1] / t, v[2] / t);
}

inline vec3 operator*(float t, vec3 v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}

inline float dot(const vec3& v1, const vec3& v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(
        v1[1] * v2[2] - v1[2] * v2[1],
        -(v1[0] * v2[2] - v1[2] * v2[0]),
        v1[0] * v2[1] - v1[1] * v2[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

struct ray {
    ray() = default;
    ray(const vec3& a, const vec3& b)
        : A(a),
          B(b) {}

    inline vec3 origin() const { return A; }

    inline vec3 direction() const { return B; }

    inline vec3 point_at_parameter(float t) const { return A + t * B; }

    vec3 A;
    vec3 B;
};

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3  oc           = r.origin() - center;
    float a            = dot(r.direction(), r.direction());
    float b            = 2.0 * dot(oc, r.direction());
    float c            = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

struct hit_record {
    float t;
    vec3  p;
    vec3  normal;
};

struct hittable {
    virtual ~hittable() {}

    virtual bool
    hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

struct sphere : public hittable {
    sphere() = default;
    sphere(vec3 cen, float r)
        : center(cen),
          radius(r) {}

    bool
    hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    vec3  center;
    float radius;
};

bool sphere::hit(
    const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3  oc           = r.origin() - center;
    float a            = dot(r.direction(), r.direction());
    float b            = dot(oc, r.direction());
    float c            = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0.0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t      = temp;
            rec.p      = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t      = temp;
            rec.p      = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }

    return false;
}

struct hittable_list : public hittable {
    hittable_list() = default;
    explicit hittable_list(const std::vector<const hittable*>& l)
        : list(l) {}

    virtual bool
    hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    std::vector<const hittable*> list;
};

bool hittable_list::hit(
    const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool       hit_anything   = false;
    double     closest_so_far = t_max;
    for (auto& hittable : list) {
        if (hittable->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything   = true;
            closest_so_far = temp_rec.t;
            rec            = temp_rec;
        }
    }

    return hit_anything;
}

vec3 color(const ray& r, hittable& world) {
    hit_record rec;
    if (world.hit(r, 0.0, MAXFLOAT, rec)) {
        return 0.5 *
               vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    } else {
        vec3  unit_direction = unit_vector(r.direction());
        float t              = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

// Returns a vector with pointers to the supplied input
// N.B. no more mutation of the input vector is allowed
template <typename U, typename T>
std::vector<const U*> make_ptr_vec(const std::vector<T>& input) {
    std::vector<const U*> res;
    res.reserve(input.size());

    for (const auto& item : input) {
        res.push_back(static_cast<const U*>(&item));
    }

    return res;
}

int main() {
    const int nx = 200;
    const int ny = 100;

    const vec3 lower_left_corner(-2.0, -1.0, -1.0);
    const vec3 horizonal(4.0, 0.0, 0.0);
    const vec3 vertical(0.0, 2.0, 0.0);
    const vec3 origin(0.0, 0.0, 0.0);

    const std::vector<sphere> spheres = {
        sphere(vec3(0, 0, -1), 0.5),
        sphere(vec3(0, -100.5, -1), 100),
    };

    hittable_list world(make_ptr_vec<hittable>(spheres));

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            const float u = float(i) / float(nx);
            const float v = float(j) / float(ny);
            const ray   r(
                origin, lower_left_corner + u * horizonal + v * vertical);

            const vec3 col = color(r, world);

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

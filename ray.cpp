#include <iostream>

struct vec3 {
    vec3() = default;
    vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3&  operator+() const        { return *this; }
    inline       vec3   operator-() const        { return vec3(-e[0], -e[1], -e[2]); }
    inline       float  operator[](int i) const  { return e[i]; }
    inline       float& operator[](int i)        { return e[i]; }

    inline vec3& operator+=(const vec3& v2) {
        e[0] += v2[0];
        e[1] += v2[1];
        e[2] += v2[2];
        return *this;
    }

    inline vec3& operator-=(const vec3& v2) {
        e[0] -= v2[0];
        e[1] -= v2[1];
        e[2] -= v2[2];
        return *this;
    }

    inline vec3& operator*=(const vec3& v2) {
        e[0] *= v2[0];
        e[1] *= v2[1];
        e[2] *= v2[2];
        return *this;
    }

    inline vec3& operator/=(const vec3& v2) {
        e[0] /= v2[0];
        e[1] /= v2[1];
        e[2] /= v2[2];
        return *this;
    }

    inline vec3& operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    inline vec3& operator/=(const float t) {
        float k = 1.0/t;

        e[0] *= k;
        e[1] *= k;
        e[2] *= k;

        return *this;
    }

    inline float length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    inline float squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    void make_unit_vector() {
        float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1]+ e[2]*e[2]);
        e[0] *= k; e[1] *= k; e[2] *= k;
    }

    float e[3];
};

inline std::istream& operator>>(std::istream& is, vec3& t) {
    return is >> t.e[0] >> t.e[1] >> t.e[2];
}

inline std::ostream& operator<<(std::ostream& os, const vec3& t) {
    return os << t.e[0] << t.e[1] << t.e[2];
}

inline vec3 operator+(const vec3& v1, const vec3 &v2) {
    return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3& v1, const vec3 &v2) {
    return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 operator*(const vec3& v1, const vec3 &v2) {
    return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 operator/(const vec3& v1, const vec3 &v2) {
    return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline vec3 operator/(vec3 v, float t) {
    return vec3(t/v[0], t/v[1], t/v[2]);
}

inline vec3 operator*(float t, vec3 v) {
    return vec3(t*v[0], t*v[1], t*v[2]);
}

// inline vec3 operator+(vec3 v, float t) {
//     return vec3(t+v[0], t+v[1], t+v[2]);
// }
//
inline float dot(const vec3& v1, const vec3& v2) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(
          v1[1]*v2[2] - v1[2]*v2[1],
        -(v1[0]*v2[2] - v1[2]*v2[0]),
          v1[0]*v2[1] - v1[1]*v2[0]
    );
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}


struct ray {
    ray() = default;
    ray(const vec3& a, const vec3& b) : A(a), B(b) {}
    vec3 origin() const                     { return A; }
    vec3 direction() const                  { return B; }
    vec3 point_at_parameter(float t) const  { return A + t*B; }

    vec3 A;
    vec3 B;
};


vec3 color(const ray& r) {
    vec3 ud = unit_vector(r.direction());
    float t = 0.5 * (ud.y() + 1.0);
    return (1.0-t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

bool hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return discriminant > 0;
}

// vec3 color(const ray& r) {
//     if (hit_sphere(vec3(0, 0, -1), 0.5, r))
//         return vec3(1, 0, 0);
//
//     vec3 unit_direction = unit_vector(r.direction());
//     float t = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
// }

int main() {
    int nx = 200;
    int ny = 100;

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizonal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u*horizonal + v*vertical);
            vec3 col = color(r);

            int ir = int(255.99*col.r());
            int ig = int(255.99*col.g());
            int ib = int(255.99*col.b());
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

#pragma once

#include <unordered_map>
#include <vector>

template <class OBJ_TYPE>
class ObjectContainor {
public:
    template <class SUB_OBJ_TYPE>
    void addObject(const std::string& name) {
        static_assert(std::is_base_of_v<OBJ_TYPE, SUB_OBJ_TYPE>);
        auto* com = new SUB_OBJ_TYPE();
        com->setName(name);
        addObject(com);
    }

    template <class SUB_OBJ_TYPE>
    SUB_OBJ_TYPE* getObject() {
        static_assert(std::is_base_of_v<OBJ_TYPE, SUB_OBJ_TYPE>);

        size_t hasCode = typeid(SUB_OBJ_TYPE).hash_code();
        auto* obj      = getObjectByTypeId(hasCode);
        if (obj == nullptr) return obj;
        return dynamic_cast<SUB_OBJ_TYPE*>(obj);
    }

    template <class SUB_OBJ_TYPE>
    SUB_OBJ_TYPE* getObject(const std::string& name) {
        static_assert(std::is_base_of_v<OBJ_TYPE, SUB_OBJ_TYPE>);
        auto* obj = getObject(name);

        if (obj == nullptr) return obj;

        return dynamic_cast<SUB_OBJ_TYPE*>(obj);
    }

    bool addObject(OBJ_TYPE* obj) {
        auto& name   = com->getName();
        auto hasCode = typeid(*obj).hash_code();

        auto it = m_name2Object.find(name);
        if (it != m_name2Object.end()) {
            return false;
        }

        obj->addRef();
        m_objList.push(obj);
        m_name2Object[name] = obj;

        auto itHash = m_type2Object.find(hasCode);
        if (itHash == m_type2Object.end()) {
            m_type2Object[hasCode] = {obj};
        } else {
            itHash->second.push_back(obj);
        }
    }

    OBJ_TYPE* getObject(const std::string& name) {
        auto it = m_name2Object.find(name);
        if (it == m_name2Object.end()) {
            return nullptr;
        }

        return it->second;
    }

    OBJ_TYPE* getObjectByTypeId(size_t clsHasCode) {
        auto it = m_type2Object.find(clsHasCode);
        if (it == m_type2Object.end() || it->second.size() <= 0) {
            return nullptr;
        }

        return it->second[0];
    }

    bool removeObject(OBJ_TYPE* obj) {
        const auto& name = obj->getName();
        auto hasCode     = typeid(*obj).hash_code();

        auto it = m_name2Object.find(name);
        if (it == m_name2Object.end()) return false;

        obj->delRef();
        m_name2Object.erase(name);
        m_objList.erase(
            std::remove_if(m_objList.begin(), m_objList.end(), obj));

        auto hashIt = m_type2Object.find(hasCode);
        if (hashIt == m_type2Object.end()) return true;

        m_objList.erase(
            std::remove_if(hashIt.second.begin(), hashIt.second.end(), obj));

        return true;
    }

protected:
    std::vector<OBJ_TYPE*> m_objList;
    std::unordered_map<std::string, OBJ_TYPE*> m_name2Object;
    std::unordered_map<size_t, std::vector<OBJ_TYPE*>> m_type2Object;
};
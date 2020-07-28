#ifndef BOOK_RESOURCEHOLDER_HPP
#define BOOK_RESOURCEHOLDER_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>


template <typename Resource, typename Identifier>
class ResourceHolder {
public:
        void
        load(Identifier id, const std::string& filename) {
                if (auto resource{ std::make_unique<Resource>() }; resource->loadFromFile(filename)) {
                        // If loading successful, insert resource to map
                        insert_resource(id, std::move(resource));
                } else {
                        throw std::runtime_error{ "ResourceHolder::load failed to load " + filename };
                }
        }

        template <typename Parameter>
        void
        load(Identifier id, const std::string& filename, const Parameter& additional_arg) {
                if (auto resource{ std::make_unique<Resource>() }; resource->loadFromFile(filename, additional_arg)) {
                        // If loading successful, insert resource to map
                        insert_resource(id, std::move(resource));
                } else {
                        throw std::runtime_error{ "ResourceHolder::load failed to load " + filename };
                }
        }

        Resource&
        get(Identifier id) {
                if (auto iter{ m_resources.find(id) }; iter != m_resources.end()) {
                        return *iter->second;
                } else {
                        throw std::runtime_error{ "ResourceHolder::get failed: id not found" };
                }
        }

        const Resource&
        get(Identifier id) const {
                if (auto iter{ m_resources.find(id) }; iter != m_resources.end()) {
                        return *iter->second;
                } else {
                        throw std::runtime_error{ "ResourceHolder::get failed: id not found" };
                }
        }

private:
        void
        insert_resource(Identifier id, std::unique_ptr<Resource> resource) {
                // Insert and check success
                auto inserted = m_resources.insert(std::make_pair(id, std::move(resource)));
                // auto inserted = m_resources.insert({ id, std::move(resource) });
                if (!inserted.second) {
                        throw std::runtime_error{ "ResourceHolder<Resource, Identifier>::insert_resource(Identifier, "
                                                  "std::unique_ptr<Resource>): insertion failed" };
                }
        }

private:
        std::map<Identifier, std::unique_ptr<Resource>> m_resources;
};

#endif  // BOOK_RESOURCEHOLDER_HPP

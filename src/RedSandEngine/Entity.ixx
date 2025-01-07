module;
export module RedSandEngine:Entity;

import <cstdint>;
import <vector>;
import <map>;

export namespace rse
{
	using Entity = uint64_t;

	class EntityManager
	{
	public:
		Entity next()
		{
			if (_returnedEntities.size() > 0)
			{
				const Entity entity = *_returnedEntities.rbegin();
				_returnedEntities.pop_back();
				return entity;
			}
			return _next++;
		}

		void returnEntity(Entity entity)
		{
			if (entity)
				_returnedEntities.push_back(entity);
		}

	private:
		Entity _next = 1;
		std::vector<Entity> _returnedEntities;
	};

	template<class T>
	class EntityComponentStore
	{
	public:
		virtual void reserve(Entity entity) = 0;
		virtual void unreserve(Entity entity) = 0;
		virtual void set(Entity entity, const T& value) = 0;
		virtual T get(Entity entity) const = 0;
	};

	template<class T>
	class CompactStore : public EntityComponentStore<T>
	{
	public:
		void reserve(Entity entity) override
		{
			if (_indices.count(entity) > 0)
				return;

			if (_freedIndices.size() > 0)
			{
				const uint64_t index = *_freedIndices.rbegin();
				_freedIndices.pop_back();
				_indices[entity] = index;
				_inUse[index] = true;
				_components[index] = T{};
			}
			else
			{
				_components.push_back(T{});
				_inUse.push_back(true);
				_indices[entity] = _components.size() - 1;
			}
		}

		void unreserve(Entity entity) override
		{
			if (_indices.count(entity) == 0)
				return;

			const uint64_t index = _indices.at(entity);
			_inUse[index] = false;
			_freedIndices.push_back(index);
			_indices.erase(entity);
		}

		void set(Entity entity, const T& value) override
		{
			if (_indices.count(entity) == 0)
				reserve(entity);
			_components[_indices.at(entity)] = value;
		}

		T get(Entity entity) const override
		{
			return _components[_indices.at(entity)];
		}

	private:
		std::vector<T> _components;
		std::vector<bool> _inUse;
		std::vector<uint64_t> _freedIndices;
		std::map<Entity, uint64_t> _indices;
	};

	template<class T>
	class HashStore : public EntityComponentStore<T>
	{
	public:
		void reserve(Entity entity) override
		{
			if (_components.count(entity) > 0)
				return;
			_components.insert({ entity, T{ } });
		}

		void unreserve(Entity entity) override
		{
			if (_components.count(entity) == 0)
				return;
			_components.erase(entity);
		}

		void set(Entity entity, const T& value) override
		{
			_components[entity] = value;
		}

		T get(Entity entity) const override
		{
			return _components.at(entity);
		}

	private:
		std::map<Entity, T> _components;
	};
}
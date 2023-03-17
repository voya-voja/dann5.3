#pragma once
#include <memory>

#include <map>

using namespace std;

namespace dann5 {

	template <typename K, typename T>
	class Factory
	{
	public:
		static Factory<K, T>& Instance() 
		{ 
			return gFactory; 
		}

		shared_ptr<T> create(K key)
		{
			return mCreators[key]->create();
		}


		template<typename C_K, typename C_T>
		class Creator
		{
		public:
			Creator(C_K key)
				:mKey(key)
			{
				Factory<C_K, C_T>::Instance().add(this);
			}

			~Creator() {
			}

			virtual shared_ptr<C_T> create() = 0;

			C_K key() { return mKey; }

		protected:
		private:
			C_K mKey;
		};

	protected:
	private:
		Factory() { }
		~Factory()
		{
#if FACTORY_DELETE_CREATORS
			std::map<K, CreatorB<K, T>*>::template iterator at(mCreators.begin());
			for (; at != mCreators.end(); at++)
			{
				CreatorB<K, T>* pCreator = at->second;
				try { delete pCreator; } // delete allocated Creators
				catch (...) {}	// otherwise ignore those that are not allocated
			}
#endif
		}

		void add(Creator<K, T>* creator)
		{
			mCreators[creator->key()] = creator;
		}
#ifdef _WINDOWS
		static Factory<K, T>		gFactory;
#else
		static inline Factory<K, T>		gFactory;
#endif
		std::map<K, Creator<K, T>*>	mCreators;
	};

	template<typename K, typename T, typename O>
	class Creator : Factory<K,T>::template Creator<K, T>
	{
	public:
		Creator(K key)
			:Factory<K,T>::template Creator<K,T>(key)
		{}

		virtual shared_ptr<T> create()
		{
			return(shared_ptr<T>(new O()));
		}

	protected:
	private:
	};
};

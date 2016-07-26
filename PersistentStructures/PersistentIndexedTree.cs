using System;

namespace DataStructures.PersistentIndexedTree
{
	public class PersistentIndexedTree<T>
	{
		private T value;

		private int level;

		private PersistentIndexedTree<T> left;
		private PersistentIndexedTree<T> right;

		private Func<T, T, T> function;

		private PersistentIndexedTree(T value, Func<T, T, T> f)
		{
			this.value = value;
			this.level = 0;
			this.function = f;
			this.left = null;
			this.right = null;
		}

		private PersistentIndexedTree(int level, Func<T, T, T> f, PersistentIndexedTree<T> left, PersistentIndexedTree<T> right)
		{
			this.value = f(left.value, right.value);

			this.level = level;
			this.function = f;

			this.left = left;
			this.right = right;
		}

		public PersistentIndexedTree(int levels, Func<T, T, T> f, T default_value)
		{
			if(levels < 0 || levels > 62) // long has 64 bits, 62 just in case
			{
				throw new IndexOutOfRangeException("Invalid number of levels");
			}

			this.level = levels;
			this.function = f;

			if(levels == 0)
			{
				this.value = default_value;

				this.left = null;
				this.right = null;

				return;
			}

			var child = new PersistentIndexedTree<T>(levels - 1, f, default_value);
			this.value = f(child.value, child.value);

			this.left = child;
			this.right = child;
		}

		public PersistentIndexedTree<T> Update(long index, T value)
		{
			if(index < 0)
			{
				throw new IndexOutOfRangeException("Index too small");
			}

			if(this.level == 0)
			{
				if(index > 0)
				{
					throw new IndexOutOfRangeException("Index too large");
				}

				return new PersistentIndexedTree<T>(value, this.function);
			}

			long middle = 1L << (this.level - 1);

			if(index < middle)
			{
				return new PersistentIndexedTree<T>(this.level, this.function, this.left.Update(index, value), this.right);
			}
			else
			{
				return new PersistentIndexedTree<T>(this.level, this.function, this.left, this.right.Update(index - middle, value));
			}
		}

		public T AtIndex(long index)
		{
			if(index < 0)
			{
				throw new IndexOutOfRangeException("Index too small");
			}

			if(this.level == 0)
			{
				if(index > 0)
				{
					throw new IndexOutOfRangeException("Index too large");
				}

				return this.value;
			}

			long middle = 1L << (this.level - 1);

			if(index < middle)
			{
				return this.left.AtIndex(index);
			}
			else
			{
				return this.right.AtIndex(index - middle);
			}
		}

		public T Query(long from, long to)
		{
			// TODO: validations

			if(from == 0L && to == (1L << this.level))
			{
				return this.value;
			}

			long middle = 1L << (this.level - 1);

			if(to <= middle)
			{
				return this.left.Query(from, to);
			}
			if(middle <= from)
			{
				return this.right.Query(from - middle, to - middle);
			}
			return this.function(this.left.Query(from, middle), this.right.Query(0, to - middle));
		}
	}
}

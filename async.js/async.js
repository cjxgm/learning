
function Async() {
	function Main(iter)
	{
		var my = {};

		my.iter = iter;

		my.each = function(iter, callback) {
			(function next(iter) {
				iter(function(value, iter) {
					callback(value, function() {
						next(iter);
					});
				});
			})(iter);
		}

		return my;
	}

	function Iter()
	{
		var my = {};

		// a closed-open-range iterator
		my.range = function(from, to, step) {
			if (step === undefined) step = 1;
			return (function iter() {
				if (from >= to) return function() { return true; };
				return function(callback) {
					var value = from;
					from += step;
					callback(value, iter());
				};
			})();
		}

		// array iterator
		my.array = function(array) {
			return (function iter(range) {
				return function(callback) {
					var end = range(function(i, range) {
						callback(array[i], iter(range));
					});
					return end;
				};
			})(my.range(0, array.length));
		}

		// iterate 2 iterators by sequence
		my.couple = function(iter1, iter2) {
			return (function iter(next_iter) {
				return function(callback) {
					var end = next_iter(function(value, next_iter) {
						callback(value, iter(next_iter));
					}) || iter2(function(value, next_iter) {
						callback(value, iter(next_iter));
					});
					if (end)
				};
			})(iter1);
		}

		my.next = function(iter, next) {
			return (function iter(next_iter) {
				return function(callback) {
					var end = next_iter(function(value, next_iter) {
						callback(value, iter(next_iter));
					});
					if (end) next();
					return end;
				};
			})(iter);
		}

		// iterate through an array of iterators by sequence
		my.seq = function(iters) {
			return (function iter(array) {
				return function(callback) {
					array(function(next_iter, array) {
						my.next(next_iter, function() {
							iter(array);
						});
					});
				};
			})(my.array(iters));
		}

		return my;
	}

	return Main(Iter());
}

var async = Async();


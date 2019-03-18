import sherpa



parameters = [sherpa.Choice('num_units', [100, 200, 300]),]

alg = sherpa.algorithms.RandomSearch(max_num_trials=150)


rval = sherpa.optimize(parameters=parameters,
                       algorithm=alg,
                       lower_is_better=True,  # Minimize objective
                       filename='./trial.py', # Python script to run, where the model was defined
                       scheduler=sherpa.schedulers.LocalScheduler(), # Run on local machine
                       )


parameters = [sherpa.Continuous('lrinit', [0.01, 0.0001], 'log'),
              sherpa.Continuous('momentum', [0., 0.99]),
              sherpa.Ordinal(name='batch_size', range=[20, 30, 100, 200, 1000, 2000, 4000])]

pbt_ranges = {'lr':[0.0000001, 1.], 'batch_size':[20, 30, 100, 200, 1000, 2000, 4000]}
algorithm = sherpa.algorithms.PopulationBasedTraining(population_size=50,
                                                      parameter_range=pbt_ranges)


sherpa.optimize(algorithm=algorithm,
                scheduler=scheduler,
                parameters=parameters,
                lower_is_better=True,
                filename="mnist_cnn.py",
                output_dir='./output')
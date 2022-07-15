classdef LinearRegression
    % 多元线性回归模型，用来对数据集进行多元线性回归

    properties
        xData, yData;   % 数据集
        eta;            % 学习率
        tol;            % 两次迭代代价函数之差 < tol，则迭代终止
        maxIterationNum;% 最大迭代次数
        w, b;           % 权重，偏置
        % y = w*(x') + b
        % x: [1 × n]，n 为数据点的自变量个数
        % w: [1 × n]
        % b: [1 × 1]
        % y: [1 × 1]
    end

    methods

        function self = LinearRegression(eta, tol, maxIterationNum)
            % 类的构造函数，初始化参数
            self.eta = eta;
            self.tol = tol;
            self.maxIterationNum = maxIterationNum;
        end
        

        function self = Fit(self, xData, yData, w0, b0)
            % 拟合数据，并将数据和拟合结果存储到自己的数据集中
            % xData, yData 为数据集
            % w0, b0 为迭代的起始点
            self.xData = xData;
            self.yData = yData;
            % 梯度下降法
            [self.w, self.b] = GradientDescent(self, w0, b0);
        end
        

        function [w, b] = GradientDescent(self, w0, b0)
            % 梯度下降法求解参数w，b
            % w0, b0 为迭代的起始点
            w = w0;
            b = b0;

            for i = 1:self.maxIterationNum
                % 求出梯度，并对其进行归一化
                % 将delta_w, delta_b 合并成一个向量后，其模 = 1
                [delta_w, delta_b] = Gradient(self, w, b);
                
                cost1 = Cost(self, w, b);   % 之前的代价
                w = w - self.eta .* delta_w;
                b = b - self.eta .* delta_b;
                cost2 = Cost(self, w, b);   % 梯度下降之后的代价
                
                % if mod(i, 100000) == 0 % main3中迭代次数过多，可取消注释，降低打印频率
                    % 打印训练进度
                    fprintf("第 %d 次迭代，cost = %.4f\n", i, cost2);
                % end

                if abs(cost1 - cost2) < self.tol
                    % 两次迭代代价函数之差 < tol，则迭代终止
                    break;
                end
            end

            if cost1 < cost2
                % 如果迭代终止之后，最后一次梯度下降没有起到
                % 减小代价函数的作用，则回到刚才的点
                w = w + self.eta .* delta_w;
                b = b + self.eta .* delta_b;
            end

        end
        

        function [delta_w, delta_b] = Gradient(self, w, b)
            % 在 (w, b) 点求梯度
            n = length(w);  % 数据点的自变量个数
            delta_w = zeros(1, n);

            h = 0.01; % 数值微分步长
            for i = 1:n     % 遍历 w 的每一个变量
                w_ = w;
                w_(i) = w_(i) - h;  % 向后差商
                cost1 = Cost(self, w_, b);
                
                w_(i) = w_(i) + 2 * h;  % 向前差商
                cost2 = Cost(self, w_, b);
                
                % 中心差商
                delta_w(i) = (cost2 - cost1) / (2 * h);
            end

            b_ = b;
            b_ = b_ - h;    % 操作同 w
            cost1 = Cost(self, w, b_);
            b_ = b_ + 2 * h;
            cost2 = Cost(self, w, b_);
            delta_b = (cost2 - cost1) / (2 * h);

            delta = [delta_w, delta_b]; % 将 w 和 b 合并为一个向量
            delta = delta ./ norm(delta, 2);    % 归一化
            
            delta_w = delta(1:n);
            delta_b = delta(n + 1);
        end
        
        
        function C = Cost(self, w, b)
            % 代价函数
            m = length(self.yData); % 数据集个数
            y_predict = self.xData * (w') + b;  % 预测值
            
            % C = Σ(y_predict - y)^2 / (2*m)
            C = dot(y_predict - self.yData, y_predict - self.yData) / (2 * m);
        end
        

        function y = Predict(self, x)
            % 对数据 x 进行预测
            y = x * (self.w') + self.b;
        end
    end

end
